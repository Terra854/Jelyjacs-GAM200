﻿
#include <Core_Engine.h>
#include <GLWindow.h>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
   CoreEngine*engine = new CoreEngine();
   GLWindow* windows = new GLWindow(1920, 1080, "Game");
   GLApp* graphics = new GLApp();                                          // @GuoChen your graphics systems nid to match this code
                                                                                 // @GuoChen Then declare ur window properties
   engine->AddSystem(windows);                                                   // @GuoChen
   engine->AddSystem(graphics);                                                  // @GuoChen

   engine->Initialize();
   windows->ActivateWindow();                                                    // @GuoChen Window Class nid this function
   windows->print_specs();
   engine->GameLoop();
   //const int targetFPS = 60; // Your desired FPS
   //const std::chrono::milliseconds frameDuration(1000 / targetFPS);
   //auto frameStart = std::chrono::high_resolution_clock::now();
   
   /*
   auto frameEnd = std::chrono::high_resolution_clock::now();
   auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

   if (frameTime < frameDuration) {
        std::this_thread::sleep_for(frameDuration - frameTime);
   }
   */
   engine->DestroySystem();
   delete engine;
}
