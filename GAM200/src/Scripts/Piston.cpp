#include "Scripts/Piston.h"
#include <Object.h>
#include <GameLogic.h>
#include <iostream>


Piston::Piston(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}

void Piston::Start(Object* obj) {
	std::cout << "Piston Script Ready : " << obj->GetName() << std::endl;
}

void Piston::Update(Object* obj) {
	
}

void Piston::Shutdown(Object* obj) {
	std::cout << "Piston Script Shutdown : " << obj->GetName() << std::endl;
}

Piston piston("Piston");