#pragma once
/* !
@file
@author	t.yeeann@digipen.edu
@date	28/9/2023


*//*__________________________________________________________________________*/
#include "Component.h"

// Nothing for now, just letting the system know that this object is
// controlled by the player
class PlayerControllable : public Component {
public:
	PlayerControllable() : Component() {};
	virtual ComponentType TypeId() const override { return ComponentType::PlayerControllable; }
};