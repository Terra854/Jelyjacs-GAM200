
#include <graphic.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "input.h"
/*  
----------------------------------------------------------------------------- */
GLint Graphic::width;
GLint Graphic::height;
GLdouble Graphic::fps;
GLdouble Graphic::delta_time;
std::string Graphic::title;
GLFWwindow* Graphic::ptr_window;


bool Graphic::init(GLint w, GLint h, std::string t) {
    Graphic::width = w;
    Graphic::height = h;
    Graphic::title = t;

    if (!glfwInit()) {
        std::cout << "GLFW init has failed - abort program!!!" << std::endl;
        return false;
    }
    // In case a GLFW function fails, an error is reported to callback function
    glfwSetErrorCallback(Graphic::error_cb);

    // Before asking GLFW to create an OpenGL context, we specify the minimum constraints
    // in that context:

    // specify OpenGL version 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // specify modern OpenGL only - no compatibility with "old" OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // applications will be double-buffered ...
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    // default behavior: colorbuffer is 32-bit RGBA, depthbuffer is 24-bits
    // don't change size of window
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // size of viewport: width x height
    Graphic::ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!Graphic::ptr_window) {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        return false;
    }

    // make the previously created OpenGL context current ...
    glfwMakeContextCurrent(Graphic::ptr_window);

    // set callback for events associated with window size changes; keyboard;
   // mouse buttons, cursor position, and scroller
    glfwSetFramebufferSizeCallback(Graphic::ptr_window, Graphic::fbsize_cb);
    //glfwSetKeyCallback(Graphic::ptr_window, Graphic::key_cb);
    //glfwSetMouseButtonCallback(Graphic::ptr_window, Graphic::mousebutton_cb);
    //glfwSetCursorPosCallback(Graphic::ptr_window, Graphic::mousepos_cb);
    //glfwSetScrollCallback(Graphic::ptr_window, Graphic::mousescroll_cb);

    // this is the default setting ...
    glfwSetInputMode(Graphic::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //set callbacks for input
    input::Init(Graphic::ptr_window);

    // initialize OpenGL (and extension) function loading library
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        return false;
    }
    const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    if (glVersion) {
        std::cout << "OpenGL Version: " << glVersion << std::endl;
    }
    else {
        std::cerr << "Unable to retrieve OpenGL version" << std::endl;
        return -1;
    }
    if (GL_VERSION_4_5) {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else {
        std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
    }

    return true;
}

/*  _________________________________________________________________________ */
/*! update

@param none

@return none

For now, there is nothing to do except to write appropriate information
to window title bar.
*/
void Graphic::update() {
    // write window title with current fps ...
    std::stringstream sstr;
    sstr << std::fixed << std::setprecision(2) << Graphic::title << " | " << Graphic::fps;
    glfwSetWindowTitle(Graphic::ptr_window, sstr.str().c_str());
}

/*  _________________________________________________________________________ */
/*! draw

@param none

@return none

For now, there is nothing to do except set the back buffer fill color
as RGB(0, 1, 0).
*/
void Graphic::draw() {
    // clear colorbuffer with RGBA value in glClearColor ...
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.f, 1.f, 0.f, 1.f);
}

/*  _________________________________________________________________________ */
/*! cleanup

@param none

@return none

For now, there are no resources allocated by the application program.
The only task is to have GLFW return resources back to the system and
gracefully terminate.
*/
void Graphic::cleanup() {
    glfwTerminate();
}


/*  _________________________________________________________________________*/
/*! key_cb

@param GLFWwindow*
Handle to window that is receiving event

@param int
the keyboard key that was pressed or released

@parm int
Platform-specific scancode of the key

@parm int
GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
action will be GLFW_KEY_UNKNOWN if GLFW lacks a key token for it,
for example E-mail and Play keys.

@parm int
bit-field describing which modifier keys (shift, alt, control)
were held down

@return none

This function is called when keyboard buttons are pressed.
When the ESC key is pressed, the close flag of the window is set.
*/
void Graphic::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
    if (GLFW_PRESS == action) {
#ifdef _DEBUG
        std::cout << "Key pressed" << std::endl;
#endif
    }
    else if (GLFW_REPEAT == action) {
#ifdef _DEBUG
        std::cout << "Key repeatedly pressed" << std::endl;
#endif
    }
    else if (GLFW_RELEASE == action) {
#ifdef _DEBUG
        std::cout << "Key released" << std::endl;
#endif
    }

    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(pwin, GLFW_TRUE);
    }
}

/*  _________________________________________________________________________*/
/*! mousebutton_cb

@param GLFWwindow*
Handle to window that is receiving event

@param int
the mouse button that was pressed or released
GLFW_MOUSE_BUTTON_LEFT and GLFW_MOUSE_BUTTON_RIGHT specifying left and right
mouse buttons are most useful

@parm int
action is either GLFW_PRESS or GLFW_RELEASE

@parm int
bit-field describing which modifier keys (shift, alt, control)
were held down

@return none

This function is called when mouse buttons are pressed.
*/
void Graphic::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
        std::cout << "Left mouse button ";
#endif
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
        std::cout << "Right mouse button ";
#endif
        break;
    }
    switch (action) {
    case GLFW_PRESS:
#ifdef _DEBUG
        std::cout << "pressed!!!" << std::endl;
#endif
        break;
    case GLFW_RELEASE:
#ifdef _DEBUG
        std::cout << "released!!!" << std::endl;
#endif
        break;
    }
}

/*  _________________________________________________________________________*/
/*! mousepos_cb

@param GLFWwindow*
Handle to window that is receiving event

@param double
new cursor x-coordinate, relative to the left edge of the client area

@param double
new cursor y-coordinate, relative to the top edge of the client area

@return none

This functions receives the cursor position, measured in screen coordinates but
relative to the top-left corner of the window client area.
*/
void Graphic::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
#ifdef _DEBUG
    std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
}

/*  _________________________________________________________________________*/
/*! mousescroll_cb

@param GLFWwindow*
Handle to window that is receiving event

@param double
Scroll offset along X-axis

@param double
Scroll offset along Y-axis

@return none

This function is called when the user scrolls, whether with a mouse wheel or
touchpad gesture. Although the function receives 2D scroll offsets, a simple
mouse scroll wheel, being vertical, provides offsets only along the Y-axis.
*/
void Graphic::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset) {
#ifdef _DEBUG
    std::cout << "Mouse scroll wheel offset: ("
        << xoffset << ", " << yoffset << ")" << std::endl;
#endif
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
void Graphic::error_cb(int error, char const* description) {
    std::cerr << "GLFW error: " << description << std::endl;
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
void Graphic::fbsize_cb(GLFWwindow* ptr_win, int width, int height) {
    // use the entire framebuffer as drawing region
    glViewport(0, 0, width, height);
    // later, if working in 3D, we'll have to set the projection matrix here ...
}

/*  _________________________________________________________________________*/
/*! update_time

@param double
fps: computed frames per second

@param double
fps_calc_interval: the interval (in seconds) at which fps is to be
calculated

@return none

This function must be first called once during initialization and once
per game loop. It uses GLFW's time functions to compute:
1. interval in seconds between each frame
2. frames per second every "fps_calc_interval" seconds
*/
void Graphic::update_time(double fps_calc_interval) {
    // get elapsed time (in seconds) between previous and current frames
    static double prev_time = glfwGetTime();
    double curr_time = glfwGetTime();
    Graphic::delta_time = curr_time - prev_time;
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
        Graphic::fps = count / elapsed_time;
        start_time = curr_time;
        count = 0.0;
    }
}
