#include <message.h>

enum Movement_direction {
	none = 0,
	up,
	down,
	left,
	right
};
class MovementKey : public Message {
public:
	MovementKey() : Message(MessageID::Movement){}
	MovementKey(Movement_direction key) : Message(MessageID::Movement) , dir(key){}
	Movement_direction dir;
};
/*
class MouseClick : Message {
	MouseClick() : Message(MessageID::MouseMove){}
	bool
};
*/