#pragma once
#include "Input.h"
#include "Time.h"
#include "Collisions.h"

class GameObject;

class Script
{

public:
	Script(GameObject* game_object)
	{
		this->game_object = game_object;
	}

	GameObject* game_object;
	
	virtual void update()
	{

	}
	virtual void late_update()
	{

	}
	virtual void fixed_update()
	{

	}

	virtual void on_collision_enter(Collision info)
	{

	}
	virtual void on_collision_stay(Collision info)
	{

	}
	virtual void on_collision_exit(Collision info)
	{

	}
	virtual void on_trigger_enter(Collider info)
	{

	}
	virtual void on_trigger_stay(Collider info)
	{

	}
	virtual void on_trigger_exit(Collider info)
	{

	}

};

