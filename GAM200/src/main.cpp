
#include <GLWindow.h>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
   Engine*engine = new Engine();
   engine->init();
   const int targetFPS = 60; // Your desired FPS
   const std::chrono::milliseconds frameDuration(1000 / targetFPS);

   while (Engine::gamestate != Gamestate::end) {
       auto frameStart = std::chrono::high_resolution_clock::now();
       engine->update();
       engine->draw();
      
       auto frameEnd = std::chrono::high_resolution_clock::now();
       auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

       if (frameTime < frameDuration) {
           std::this_thread::sleep_for(frameDuration - frameTime);
       }
   }
  
   engine->cleanup();
}
