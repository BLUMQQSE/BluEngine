#pragma once

namespace bm98
{
class GameObject;
class Entity;

class Component
{
public:
	Component();
	~Component();

	virtual void init();
	virtual void update();
	virtual void late_update();
	virtual void fixed_update();
	virtual void add_to_buffer(sf::View* view = nullptr);

	virtual GameObject* get_game_object();

	GameObject* game_object;

protected:
	//Entity* game_object;
};

}