#pragma once
/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>
#include <string>
#include <message.h>

//Base System class for all systems - Interface.
class ISystems {
public:
	ISystems() {};
	virtual ~ISystems() {};

	//Messaging System for inter System communication
	virtual void MessageRelay(Message_Handler* msg) {};

	// System Initialization
	virtual void Initialize(){};

	// System Update
	virtual void Update(float time) = 0;


	// System freeing resources
	virtual void Free(){};

	//For Debugging, getting System Name
	virtual std::string SystemName() { return "ISystems (If you are seeing this, you need to implement SystemName() inside yor component)"; };
};