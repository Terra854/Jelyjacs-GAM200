
#include <graphic.h>
#include <engine.h>
#include <iostream>


int main() {
   Engine::init();
  
   while (Engine::gamestate != Gamestate::end) {
       Engine::draw();
       Engine::update();
   }
  
   Engine::cleanup();
}
