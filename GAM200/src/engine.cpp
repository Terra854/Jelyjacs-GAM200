#include <engine.h>
#include <iostream>

Gamestate Engine::gamestate = Gamestate::start;

void Engine::init() {
    
    if (!Graphic::init(2400, 1350, "Window")) {
        std::cout << "Unable to create window" << std::endl;
        std::exit(EXIT_FAILURE);
    }

}

void Engine::update(){
    // process events if any associated with input devices
    glfwPollEvents();
    // main loop computes fps and other time related stuff once for all apps ...
    Graphic::update_time(1.0);
    // animate scene
    Graphic::update();
    if(glfwWindowShouldClose(Graphic::ptr_window)){
		gamestate = Gamestate::end;
	}
}

void Engine::draw() {
    // render scene
    Graphic::draw();

    // swap buffers: front <-> back
    // GLApp::ptr_window is handle to window that defines the OpenGL context
    glfwSwapBuffers(Graphic::ptr_window);
}

void Engine::cleanup() {
    Graphic::cleanup();
	
}