#pragma once
/* !
@file
@author
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>

#include "component.h"

class BaseComponentCreator {
public:
	Component* Create();
	template <typename... Args> Component* Create(Args&&... args);
	virtual ~BaseComponentCreator() {};
};

//used by addcomponent function that specifies what type of game component to create
template<typename ComponentType> class ComponentCreator : public BaseComponentCreator
{
public:
	ComponentCreator() {}
	~ComponentCreator() {}

	Component* Create()
	{
		return new ComponentType();
	}

	template <typename... Args> Component* Create(Args&&... args)
	{ 
		return new ComponentType(std::forward<Args>(args)...);
	}
};