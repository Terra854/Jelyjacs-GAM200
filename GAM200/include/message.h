#pragma once
/* !
@file message.h
@author Luke Goh
@date	28/9/2023

This file contains the declaration for the Message Handler class and an enum of Event_Types
*//*__________________________________________________________________________*/
#include <Debug.h>

/****************************************************************************
* MessageID
*  - Different types of Events that systems need to communicate around
*****************************************************************************/
namespace MessageID {
	enum Event_Type
	{
		None,
		Quit,
		Collide,
		Movement,
		MouseMove, 
		MouseClick,
		NoMovement
	};
}
/****************************************************************************
* Message Handler
* - Parent Class for Messages
* - Store message
*  - Allow system to read the message but not edit it
*****************************************************************************/
class Message_Handler
{
public:
	// Default Constructor
	Message_Handler() { message = MessageID::Event_Type::None; }

	// Constructor
	Message_Handler(MessageID::Event_Type event) : message(event) {};

	// Function to get message
	MessageID::Event_Type GetMessage() { return message; }

private:
	MessageID::Event_Type message;
};