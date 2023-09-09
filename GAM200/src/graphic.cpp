#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <graphic.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>
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
GLSLShader Graphic::shdr_pgm{};

//vector of objects
std::vector<Graphic::Object> Graphic::all_objects;

std::vector <glm::mat4> Graphic::ptr_trans;



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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

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
    CreatObject("../Assest/Picture/test.png", glm::vec2(0.f, 0.f), glm::vec2(0.5f, 0.5f), 0.5f, "test");

    return true;
}

void Graphic::setup_quad_vao() {
    //vertices
    std::array <glm::vec2, 4 > pos_vtx
    {
        glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 1.0f),
            glm::vec2(-1.0f, 1.0f), glm::vec2(-1.0f, -1.0f)
    };
    std::array <glm::vec3, 4 > clr_vtx
    {
        glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)
    };

    std::array <glm::vec2, 4 > tex_coor
    {
        glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f)
    };

    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size() + sizeof(glm::vec2) * tex_coor.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vbo, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
    glNamedBufferSubData(vbo, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());
    glNamedBufferSubData(vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
        sizeof(glm::vec2) * tex_coor.size(), tex_coor.data());

    GLuint vaoid;
    glCreateVertexArrays(1, &vaoid);

    glEnableVertexArrayAttrib(vaoid, 0);
    glVertexArrayVertexBuffer(vaoid, 0, vbo, 0, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoid, 0, 0);

    glEnableVertexArrayAttrib(vaoid, 1);
    glVertexArrayVertexBuffer(vaoid, 1, vbo, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
    glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoid, 1, 1);

    glEnableVertexArrayAttrib(vaoid, 2);
    glVertexArrayVertexBuffer(vaoid, 2, vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(), sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoid, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoid, 2, 2);

    std::array<GLushort, 6> idx_vtx
    {
            0, 1, 2,
            2, 3, 0
    };

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl, sizeof(GLshort) * idx_vtx.size(), idx_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vaoid, ebo_hdl);
    glBindVertexArray(0);

}

void Graphic::setup_shdrpgm()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(
        GL_VERTEX_SHADER,
        "../shaders/image.vert"));
    shdr_files.push_back(std::make_pair(
        GL_FRAGMENT_SHADER,
        "../shaders/image.frag"));
    shdr_pgm.CompileLinkValidate(shdr_files);
    if (GL_FALSE == shdr_pgm.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << shdr_pgm.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
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
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.image_width, image.image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image_data);
        //glGenerateMipmap(GL_TEXTURE_2D);

        GLuint texobj_hdl;
        // define and initialize a handle to texture object that will
        // encapsulate two-dimensional textures
        glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);
        // allocate GPU storage for texture image data loaded from file
        glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, image.image_width, image.image_height);
        // copy image data from client memory to GPU texture buffer memory
        glTextureSubImage2D(texobj_hdl, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.image_data);
    }
    Object object(image, position_input, scale_input, rotation_input, name);
    all_objects.push_back(object);
    stbi_image_free(image.image_data);

    //set up trans
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(position_input, 0.0f));
    trans = glm::rotate(trans, glm::radians(rotation_input), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(scale_input, 1.0f));
    ptr_trans.push_back(trans);
}

void Graphic::update() {
    
    std::stringstream sstr;
    sstr << std::fixed << std::setprecision(2) << Graphic::title << " | " << Graphic::fps;
    glfwSetWindowTitle(Graphic::ptr_window, sstr.str().c_str());

   
}


void Graphic::draw() {

    
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // bind Texture
    for (auto texture : ptr_tex) {
     
        glBindTexture(GL_TEXTURE_2D, texture);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        shdr_pgm.Use();
        GLuint tex_loc = glGetUniformLocation(shdr_pgm.GetHandle(), "uTex2d");
        glUniform1i(tex_loc, 3);
        glBindVertexArray(vaoid);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
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

