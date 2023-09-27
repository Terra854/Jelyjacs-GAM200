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
	Message() { messageId = MessageID::MessageIDType::None; }
	Message(MessageID::MessageIDType id) : messageId(id) {};
	MessageID::MessageIDType messageId;
	virtual ~Message() {};
};