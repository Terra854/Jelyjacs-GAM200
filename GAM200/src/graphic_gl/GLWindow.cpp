/*!

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GLWindow.h>
#include <iostream>
#include "input.h"
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
// static data members declared in GLHelper
GLint GLWindow::width;
GLint GLWindow::height;
GLdouble GLWindow::fps;
GLdouble GLWindow::delta_time;
std::string GLWindow::title;
GLFWwindow* GLWindow::ptr_window;

//Gamestate Engine::gamestate = Gamestate::start;
//Global Pointer to Window System
GLWindow* window = NULL;
/*
constructor
*/
GLWindow::GLWindow(GLint w, GLint h, std::string t) {
	GLWindow::width = w;
	GLWindow::height = h;
	GLWindow::title = t;
}

/*  _________________________________________________________________________ */
/*! init

@param GLint width
@param GLint height
Dimensions of window requested by program


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

    GLWindow::ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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

}

void GLWindow::Update(float time)
{
    glfwPollEvents();

    // Part 2
    GLWindow::update_time(1.0);
}


void GLWindow::ActivateWindow()
{

}


/*  _________________________________________________________________________ */
/*! cleanup

@param none

@return none

For now, there are no resources allocated by the application program.
The only task is to have GLFW return resources back to the system and
gracefully terminate.
*/
void GLWindow::cleanup() {
    // Part 1
    glfwTerminate();
}


/*  _________________________________________________________________________ */
/*! error_cb

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
#ifdef _DEBUG
    std::cerr << "GLFW error: " << description << std::endl;
#endif
}

/*  _________________________________________________________________________ */
/*! fbsize_cb

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
    // later, if working in 3D, we'll have to set the projection matrix here ...
}

/*  _________________________________________________________________________*/
/*! update_time

@param double
fps_calc_interval: the interval (in seconds) at which fps is to be
calculated

This function must be called once per game loop. It uses GLFW's time functions
to compute:
1. the interval in seconds between each frame
2. the frames per second every "fps_calc_interval" seconds
*/
void GLWindow::update_time(double fps_calc_interval) {
    // get elapsed time (in seconds) between previous and current frames
    static double prev_time = glfwGetTime();
    double curr_time = glfwGetTime();
    delta_time = curr_time - prev_time;
    prev_time = curr_time;

    // fps calculations
    static double count = 0.0; // number of game loop iterations
    static double start_time = glfwGetTime();
    // get elapsed time since very beginning (in seconds) ...
    double elapsed_time = curr_time - start_time;

    ++count;

    // update fps at least every 10 seconds ...
    fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
    fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
    if (elapsed_time > fps_calc_interval) {
        GLWindow::fps = count / elapsed_time;
        start_time = curr_time;
        count = 0.0;
    }
}

/*  _________________________________________________________________________*/
/*! print_specs()

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

