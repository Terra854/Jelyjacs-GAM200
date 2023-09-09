#include <graphic.h>
#include <glhelper.h>
#include <glapp.h>
#include <iostream>
enum class Gamestate{
	start,
	end
};

struct Engine {
	static void init();
	static void update();
	static void draw();
	static void cleanup();

	static Gamestate gamestate;
	
};