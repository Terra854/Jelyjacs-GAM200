#include <glm/glm.hpp>
#include <graphic.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*  
----------------------------------------------------------------------------- */
GLint Graphic::width;
GLint Graphic::height;
GLdouble Graphic::fps;
GLdouble Graphic::delta_time;
std::string Graphic::title;
GLFWwindow* Graphic::ptr_window; 
GLuint Graphic::vaoid;        // with GL 4.5, VBO & EBO are not required
GLuint Graphic::elem_cnt;     // how many indices in element buffer
GLuint Graphic::pboid;        // id for PBO
std::vector <GLuint> Graphic::ptr_tex;        // id for texture object

//vector of objects
std::vector<Graphic::Object> Graphic::all_objects;



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

    // this is the default setting ...
    glfwSetInputMode(Graphic::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
        return true;
    }
    
    
    setup_quad_vao();
    setup_shdrpgm();
    CreatObject("../Assest/Picture/img_1.png",glm::vec2(1.f,1.f), glm::vec2(1.f, 1.f), 0.f,"img1");
    CreatObject("../Assest/Picture/test.png", glm::vec2(1.f, 1.f), glm::vec2(1.f, 1.f), 0.f, "test");

    return true;
}

void Graphic::setup_quad_vao() {
    //vertices
    
    float vertices[] = {
        // positions       // texture coords
     1.f,  1.f, 0.0f,    1.0f, 1.0f,   // top right
     1.f, -1.f, 0.0f,    1.0f, 0.0f,   // bottom right
    -1.f, -1.f, 0.0f,    0.0f, 0.0f,   // bottom left
    -1.f,  1.f, 0.0f,    0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0,1,2,
        3,2,0
    };

    //VBO buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //bind VBO buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //configer buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //vertex array
    
    glGenVertexArrays(1, &vaoid);

    //bind vertex array
    glBindVertexArray(vaoid);

    //linking vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attributes
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //EBO buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //bind EBO buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   
}

void Graphic::setup_shdrpgm()
{
    //vertex shader code
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location=0) in vec3 aPos;\n"
        "layout (location=2) in vec2 aTexCoord;"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(aPos, 1.0);\n"
        "TexCoord = aTexCoord;\n"
        "}\0";

    //fragment shader code
    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec2 TexCoord;\n"
        "out vec4 FragColor;\n"

        "uniform sampler2D ourTexture;\n"

        "void main()\n"
        "{\n"
        "FragColor = texture( ourTexture , TexCoord );\n"
        "}\0";

    //create vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //compile vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //check if compile correctly
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //check if compile correctly
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //program object
    unsigned int Program = glCreateProgram();

    //attach shaders and link
    glAttachShader(Program, vertexShader);
    glAttachShader(Program, fragmentShader);
    glLinkProgram(Program);

    //check if linking failed
    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(Program, 512, NULL, infoLog);
        std::cout << "linking failed";
    }

    //delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(Program);
}

void Graphic::CreatObject(const char* file_name_input, glm::vec2 position_input, glm::vec2 scale_input, GLfloat rotation_input, std::string name) {
    stbi_set_flip_vertically_on_load(true);
    unsigned int texid;
    ptr_tex.push_back(texid);
    glGenTextures(1, &ptr_tex.back());
    glBindTexture(GL_TEXTURE_2D, ptr_tex.back()); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    Image image;
    image.image_data = stbi_load(file_name_input, &image.image_width, &image.image_height, &image.image_channels, STBI_rgb_alpha);


    if (!image.image_data) {
        std::cerr << "Failed to load image: " << name << "image" << std::endl;
    }
    else {
        std::cout << "Image" << name << "loaded with a width of " << image.image_width << "  and a height of " << image.image_height << " " << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.image_width, image.image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    Object object(image, position_input, scale_input, rotation_input, name);
    all_objects.push_back(object);
    stbi_image_free(image.image_data);
}

void Graphic::update() {
    
    std::stringstream sstr;
    sstr << std::fixed << std::setprecision(2) << Graphic::title << " | " << Graphic::fps;
    glfwSetWindowTitle(Graphic::ptr_window, sstr.str().c_str());

   
}


void Graphic::draw() {

    
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int i = 0;
    // bind Texture
    for (auto texture : ptr_tex) {
        if (i == 1)continue;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vaoid);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        std::cout<< "drawing" <<i<< std::endl;
        i++;
    }
    
}


void Graphic::cleanup() {
    glfwTerminate();
}



void Graphic::error_cb(int error, char const* description) {
    std::cerr << "GLFW error: " << description << std::endl;
}



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

//void Graphic::loadPicture() {
//    const char* image_file = "Picture/img_1.png";  
//    int  image_channels;
//    image_data = stbi_load(image_file, &image_width, &image_height, &image_channels, STBI_rgb_alpha);
//    if (!image_data) {
//        std::cerr << "Failed to load image: " << image_file << std::endl;
//    }
//}

//void Graphic::RenderPNGImage(unsigned char* image_data, int img_width, int img_height) {
//   
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(0, width, 0, height, -1, 1);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//    
//    glDrawPixels(img_width, img_height, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
//}