#include <iostream>
#include "Test.h"
namespace Test {
	void Start(Object* obj) {
		std::cout << "Start" << std::endl;
	}
	void Update(Object* obj) {
		std::cout << "Update" << std::endl;
	}
	void End(Object* obj) {
		std::cout << "End" << std::endl;
	}

	//Having Error, GameLogic* Logic is a nullptr
	LogicScript(Start, Update, End);
}
