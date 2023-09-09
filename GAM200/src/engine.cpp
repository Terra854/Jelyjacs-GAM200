#include <engine.h>
#include <iostream>

Gamestate Engine::gamestate = Gamestate::start;

void Engine::init() {
    
    if (!GLHelper::init(1920, 1080, "GAME")) {
        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    GLHelper::print_specs();
    GLApp::init();

}

void Engine::update(){
    
    glfwPollEvents();
    GLHelper::update_time(1.0);
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