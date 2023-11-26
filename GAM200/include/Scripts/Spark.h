#pragma once
#include <Object.h>
#include <GameLogic.h>

class Spark : public LogicScript
{
public:
	Spark(std::string);
	~Spark() = default;

	void Start(Object* obj);
	void Update(Object* obj);
	void Shutdown(Object* obj);

	bool Connected_to_Finn;
	static bool Just_detached;
};