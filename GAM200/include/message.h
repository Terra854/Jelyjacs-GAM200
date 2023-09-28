#pragma once
#include <Debug.h>

namespace MessageID {
	enum MessageIDType
	{
		None,
		Quit,
		Collide,
		Movement,
		MouseMove, 
		MouseClick
	};
}

class Message
{
public:
	Message() { message_id = MessageID::MessageIDType::None; }
	Message(MessageID::MessageIDType id) : message_id(id) {};
	MessageID::MessageIDType message_id;
	virtual ~Message() {};
};