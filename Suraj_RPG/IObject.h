#pragma once
#include "pch.h"

#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"
#include "AnimationComponent.h"
#include "ChildAnimationComponent.h"
#include "AnimatedSpriteComponent.h"
namespace bm98
{
class IObject
{
public:

	virtual void add_to_buffer(sf::View* view = nullptr)
	{

	}
	virtual void update()
	{

	}
	virtual void fixed_update()
	{

	}
	virtual void late_update()
	{

	}

};
}