#include <engine.h>
#include <iostream>

//need to use our own math
#include <glm/gtc/type_ptr.hpp>
/*
Gamestate Engine::gamestate = Gamestate::start;


void Engine::init() {
    
    if (!GLHelper::init(1920, 1080, "GAME")) {
        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    GLHelper::print_specs();
    GLApp::init();

}

float x = 0.f;
float y = 0.f;
void Engine::update(){
    
    x += 0.01f;
    y += 0.01f;
    glfwPollEvents();
    GLHelper::update_time(1.0);

    
    GLApp::objects["object1"].position = { x,y };
    
    GLApp::objects["object2"].orientation = { x };
    GLApp::update();
    

}

void Engine::draw() {
    
    GLApp::draw();

    glfwSwapBuffers(GLHelper::ptr_window);
}

void Engine::cleanup() {
    
    GLApp::cleanup();

    GLHelper::cleanup();
}
*/