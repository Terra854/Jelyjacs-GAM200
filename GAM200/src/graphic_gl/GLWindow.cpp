/*!
@file    GLWindow.cpp
@author  Guo Chen (g.chen@digipen.edu)
@date    28/09/2023

Window system for the engine
includes all the functions to create and update the window

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <Debug.h>
#include <GLWindow.h>
#include <iostream>
#include "input.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include "stb_image.h"
#include "Core_Engine.h"
#include <SceneManager.h>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
// static data members declared in
GLint GLWindow::width;
GLint GLWindow::height;
GLint GLWindow::width_init;
GLint GLWindow::height_init;
GLdouble GLWindow::fps;
GLdouble GLWindow::delta_time;
std::string GLWindow::title;
GLFWwindow* GLWindow::ptr_window;
GLint GLWindow::width_windowed;
GLint GLWindow::height_windowed;
Window_size GLWindow::window_size;
const GLFWvidmode* originalMode;
int originalX, originalY, originalWidth, originalHeight;
GLFWmonitor* monitor;

//Global Pointer to Window System
GLWindow* window = NULL;

/*  _________________________________________________________________________ */
/*
constructor
*/
GLWindow::GLWindow() {

    std::ifstream ifs("scenes/config.txt", std::ios::in);
    if (!ifs)
    {
        std::cout <<
            "Error : Unable to open config file : " << std::endl;
        exit(EXIT_FAILURE);
    }
    ifs.seekg(0, std::ios::beg);
    std::string line;
    getline(ifs, line);
    std::istringstream line_title{ line };
    line_title >> GLWindow::title;
    getline(ifs, line);
    std::istringstream line_width{ line };
    line_width >> GLWindow::width_init;
    getline(ifs, line);
    std::istringstream line_height{ line };
    line_height >> GLWindow::height_init;
    getline(ifs, line);
    std::istringstream line_icon{ line };
    line_icon >> GLWindow::icon_path;

    ifs.close();
    width = width_init;
    height = height_init;

   
}

/*  _________________________________________________________________________ */
/*! initialize GLFW and GLEW
*/

void GLWindow::Initialize() {
    
    // Part 1
    if (!glfwInit()) {
        std::cout << "GLFW init has failed - abort program!!!" << std::endl;
        return;
    }

    // In case a GLFW function fails, an error is reported to callback function
    glfwSetErrorCallback(GLWindow::error_cb);

    // Before asking GLFW to create an OpenGL context, we specify the minimum constraints
    // in that context:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

    GLWindow::ptr_window = glfwCreateWindow(width_init, height_init, title.c_str(), NULL, NULL);
    if (!window->ptr_window) {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(GLWindow::ptr_window);

    glfwSetFramebufferSizeCallback(GLWindow::ptr_window, GLWindow::fbsize_cb);
    input::Init(GLWindow::ptr_window);

    // this is the default setting ...
    glfwSetInputMode(GLWindow::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Part 2: Initialize entry points to OpenGL functions and extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        return;
    }
    if (GL_VERSION_4_5) {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else {
        std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
        return;
    }
    window_size = Window_size::high;
    monitor = glfwGetPrimaryMonitor();
    originalMode = glfwGetVideoMode(monitor);
    glfwGetWindowPos(ptr_window, &originalX, &originalY);
    glfwGetWindowSize(ptr_window, &originalWidth, &originalHeight);

    GLFWimage images[1];
    images[0].pixels = stbi_load(icon_path.c_str(), &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(ptr_window, 1, images);
    stbi_image_free(images[0].pixels);
}

/*  _________________________________________________________________________ */
/*
* upate event and detect close button
*/
void GLWindow::Update()
{
    
    glfwPollEvents();
    glfwSetWindowIconifyCallback(ptr_window, window_iconify_callback);

    static bool first_time = true;
#ifdef _DEBUG
    first_time = false;
#endif
    if (first_time) {
        change_window_size_fullscreen();
        first_time = false;
    }
    //change to windowed mode
    if (input::IsPressed(KEY::x)) {
        if (window_size == Window_size::fullscreen)
        {
			window_size = Window_size::high;
            // restore windows desktop resolution
            glfwSetWindowMonitor(ptr_window, NULL, originalX, originalY, originalWidth, originalHeight, 0);
		}
        else {
           //switch to another resolution
            switch (window_size)
            {
			case Window_size::high:
				window_size = Window_size::medium;
				break;
			case Window_size::medium:
				window_size = Window_size::low;
				break;
			case Window_size::low:
				window_size = Window_size::high;
				break;
			}
        }
        int pos_x, pos_y;
        switch (window_size)
        {
        case Window_size::high:
            width = 1600;
            height = 900;
            glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width, height, 0);
            pos_x = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - width) / 2;
            pos_y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - height) / 2;
            pos_y += 100;
            glfwSetWindowPos(ptr_window, pos_x, pos_y);
            break;
        case Window_size::medium:
            width = 1280;
            height = 720;
            glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width, height, 0);
            pos_x = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - width) / 2;
            pos_y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - height) / 2;
            glfwSetWindowPos(ptr_window, pos_x, pos_y);
            break;
        case Window_size::low:
            width = 800;
            height = 450;
            glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width, height, 0);
            pos_x = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - width) / 2;
            pos_y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - height) / 2;
            glfwSetWindowPos(ptr_window, pos_x, pos_y);
            break;
        }

    }
    //change to fullscreen mode
    if (input::IsPressed(KEY::f)) {
        window_size = Window_size::fullscreen;
        width = 1600;
        height = 900;
        glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width, height, 0);
		glfwSetWindowMonitor(ptr_window, monitor, 0, 0, originalMode->width, originalMode->height, originalMode->refreshRate);
	}
    
    // Check if the close button or alt + f4 is pressed
    if (glfwWindowShouldClose(ptr_window)) {
        Message_Handler msg(MessageID::Event_Type::Quit);
        engine->Broadcast(&msg);
    }
    
}

/*  _________________________________________________________________________ */
/*
*  change to windowed mode
* 
*/
void GLWindow::ChangeWindowMode()
{


    // Destroy the current fullscreen window
    glfwDestroyWindow(ptr_window);

    // Create a new window in windowed mode with the desired position and size
    ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    std::cout<< "Window mode changed to windowed mode" << std::endl;
    std::cout<<"width"<<width<<std::endl;
    std::cout<<"height"<<height<<std::endl;

    // Check if the window was created successfully
    if (!ptr_window) {
        // Handle the error
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

/*  _________________________________________________________________________ */
/*
* window iconify callback
* @param w window pointer
* @param iconified iconified or not
* 
*/
void GLWindow::window_iconify_callback(GLFWwindow* w, int iconified)
{
    (void)w;
    if (iconified)
	{
		// The window was iconified
		std::cout << "Window was iconified" << std::endl;
        SceneManager::PauseScene();
	}
	else
	{
		// The window was restored
		std::cout << "Window was restored" << std::endl;
        SceneManager::PlayScene();
	}

}


/*  _________________________________________________________________________ */
/*! cleanup
*/
void GLWindow::cleanup() {

    glfwTerminate();
}


/*  _________________________________________________________________________ */
/*! error_cb

@ wrote by pghali@digipen.edu

@param int
GLFW error code

@parm char const*
Human-readable description of the code

@return none

The error callback receives a human-readable description of the error and
(when possible) its cause.
*/
void GLWindow::error_cb(int error, char const* description) {
    (void)error;
    (void)description;
#ifdef _DEBUG
    std::cerr << "GLFW error: " << description << std::endl;
#endif
}

/*  _________________________________________________________________________ */
/*! fbsize_cb
* 
@ wrote by pghali@digipen.edu

@param GLFWwindow*
Handle to window that is being resized

@parm int
Width in pixels of new window size

@parm int
Height in pixels of new window size

@return none

This function is called when the window is resized - it receives the new size
of the window in pixels.
*/
void GLWindow::fbsize_cb(GLFWwindow* ptr_win, int wwidth, int hheight) {
    (void)ptr_win;
#ifdef _DEBUG
    std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
    // use the entire framebuffer as drawing region
    glViewport(0, 0, wwidth, hheight);
   
}


/*  _________________________________________________________________________*/
/*! print_specs()
* 
* @ wrote by pghali@digipen.edu

@param none

Print usefulvinformation about OpenGL context and graphics hardware to standard output
*/
void GLWindow::print_specs() {
    GLubyte const* str_vendor = glGetString(GL_VENDOR);
    GLubyte const* str_renderer = glGetString(GL_RENDERER);
    GLubyte const* str_version = glGetString(GL_VERSION);
    GLubyte const* str_shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "GPU Vendor: " << str_vendor << std::endl;
    std::cout << "GL Renderer: " << str_renderer << std::endl;
    std::cout << "GL Version: " << str_version << std::endl;
    std::cout << "GL Shader Version: " << str_shader_version << std::endl;

    // Print major and minor version
    GLint major_version, minor_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);
    std::cout << "GL Major Version: " << major_version << std::endl;
    std::cout << "GL Minor Version: " << minor_version << std::endl;

    // Print double buffering information
    GLint double_buffered;
    glGetIntegerv(GL_DOUBLEBUFFER, &double_buffered);
    std::cout << "Current OpenGL Context is " << (double_buffered ? "double buffered" : "not double buffered") << std::endl;

    // Print maximum vertex count
    GLint max_vertex_count;
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &max_vertex_count);
    std::cout << "Maximum Vertex Count: " << max_vertex_count << std::endl;

    // Print maximum indices count
    GLint max_indices_count;
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &max_indices_count);
    std::cout << "Maximum Indices Count: " << max_indices_count << std::endl;

    // Print maximum texture size
    GLint max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    std::cout << "GL Maximum texture size: " << max_texture_size << std::endl;

    // Print maximum viewport dimensions
    GLint max_viewport_dimensions[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, max_viewport_dimensions);
    std::cout << "Maximum Viewport Dimensions: " << max_viewport_dimensions[0] << " x " << max_viewport_dimensions[1] << std::endl;

}

/*  _________________________________________________________________________ */
/*
* change window size
* @param size window size
*/
void GLWindow::change_window_size(Window_size size) {
    if (window_size == Window_size::fullscreen)
    {
        // restore windows desktop resolution
        glfwSetWindowMonitor(ptr_window, NULL, originalX, originalY, originalWidth, originalHeight, 0);
    }

    window_size = size;
    int pos_x, pos_y;
    switch (window_size)
    {
    case Window_size::high:
        width = 1920;
        height = 1080;
        glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width-192, height-108, 0);
        pos_x = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - width) / 2;
        pos_y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - height) / 2;
        pos_y += 100;
        glfwSetWindowPos(ptr_window, pos_x, pos_y);
        break;
    case Window_size::medium:
        width = 1280;
        height = 720;
        glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width, height, 0);
        pos_x = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - width) / 2;
        pos_y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - height) / 2;
        glfwSetWindowPos(ptr_window, pos_x, pos_y);
        break;
    case Window_size::low:
        width = 800;
        height = 450;
        glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width, height, 0);
        pos_x = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - width) / 2;
        pos_y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - height) / 2;
        glfwSetWindowPos(ptr_window, pos_x, pos_y);
        break;
    }
}

/*  _________________________________________________________________________ */
/*
* change window size
* @param w width
* @param h height
*/
void GLWindow::change_window_size(int w, int h) {
	width = w;
	height = h;
	glfwSetWindowSize(ptr_window, width, height);
	int pos_x = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width - width) / 2;
	int pos_y = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height - height) / 2;
	glfwSetWindowPos(ptr_window, pos_x, pos_y);
}

/*  _________________________________________________________________________ */
/*
* change window size to fullscreen
*/
void GLWindow::change_window_size_fullscreen() {
    window_size = Window_size::fullscreen;
    width = width_init;
    height = height_init;
    glfwSetWindowMonitor(ptr_window, NULL, 0, 0, width, height, 0);
    glfwSetWindowMonitor(ptr_window, monitor, 0, 0, originalMode->width, originalMode->height, originalMode->refreshRate);
}