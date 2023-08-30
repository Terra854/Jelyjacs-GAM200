#include <graphic.h>

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