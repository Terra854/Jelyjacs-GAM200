#include "Test.h"


Test::Test(std::string name) : LogicScript(name) {
	std::cout << "Test Constructor" << std::endl;
}

void Test::Start(Object* obj) {
	std::cout << "Start" << obj->GetName() << std::endl;
}
void Test::Update(Object* obj) {
	std::cout << "Update" << obj->GetName() << std::endl;
}
void Test::Shutdown(Object* obj) {
	std::cout << "End" << obj->GetName() << std::endl;
}

//Test mytest("Test");
//Having Error, GameLogic* Logic is a nullptr
//LogicScript(Start, Update, End);


