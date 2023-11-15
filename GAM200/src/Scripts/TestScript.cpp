#include "Scripts/TestScript.h"

TestScript::TestScript(std::string name) : LogicScript(name) {
	std::cout << name << " Created" << std::endl;
}

void TestScript::Start(Object* obj) {
	//std::cout << "Start : " << obj->GetName() << std::endl;
}
void TestScript::Update(Object* obj) {
	//std::cout << "Update : " << obj->GetName() << std::endl;
}
void TestScript::Shutdown(Object* obj) {
	//std::cout << "End : " << obj->GetName() << std::endl;
}

TestScript mytest("Test");
