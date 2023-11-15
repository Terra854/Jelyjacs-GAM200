/* !
@file LogicScript.h
@author Luke Goh
@date	3/11/2023

This file contains the TestScript class that is used to test the LogicScript Interface
*//*__________________________________________________________________________*/
#include "Test.h"
namespace Script1 {
	Test::Test(std::string name) : LogicScript(name) {
		std::cout << name << " Created" << std::endl;
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

	Test mytest("Test");
}
