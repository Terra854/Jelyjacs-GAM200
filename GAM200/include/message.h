#pragma once
#include <Debug.h>

namespace MessageID
{
	enum MessageIDType
	{
		None,
		Quit,
		Collide,
		MouseButton_Press,
		MouseButton_Hold,
		MouseMove,
		Key_Press,
		Key_Hold
	};
}

//The message clas isn't meant to be used on it's own
//Instead, every game object will be capable of sending it's own kind of
//message and will inherit from this class in order to create them
//The ID above will show what kind of signals we want our systems to send each other
//More will be added as the game expands
class Message
{
public:
	Message(MessageID::MessageIDType id) : messageId(id) {};
	MessageID::MessageIDType messageId;
	virtual ~Message() {};
};