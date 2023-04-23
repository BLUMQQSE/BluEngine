#pragma once
#include "globals.h"
#include "Math.h"
#include "IData.h"

namespace bm98
{
class GameObject;
class Entity;
class Collision;
class Collider;

namespace Editor
{

/// <summary>
/// Struct containing a void* for a variable and a string identifier
/// for what type to convert it back to.
/// </summary>
struct SerializedVar
{
	SerializedVar(std::string name, void* variable, Var::Type type, 
		std::vector<std::string> extra_data = std::vector<std::string>())
	{
		this->name = name;
		this->variable = variable;
		this->extra_data = extra_data;
		this->type = type;
	}

	std::string name;
	void* variable;
	std::vector<std::string> extra_data;
	Var::Type type;

};
}
class Component : public IData
{

protected:

	

public:
	Component();
	virtual ~Component();

	/// <summary>
	/// First function called when a game object is being initialized in a scene.
	///  Will initialize Component with all other components it needs.
	/// </summary>
	virtual void init();
	/// <summary>
	/// Second function called when a game object is being initialized in a scene.
	/// Will wrap up any initialization needed for dedicated components. This function
	/// should be used by scripts to initialize their data.
	/// </summary>
	virtual void awake();
	/// <summary>
	/// Third and final start up function for game objects initialized. This function should
	/// solely be used by scripts, and is used to link with other scripts and dedicated components.
	/// </summary>
	virtual void start();

	virtual void on_destroy();

	virtual void update();
	virtual void late_update();
	virtual void fixed_update();
	virtual void add_to_buffer(sf::View* view = nullptr);

	virtual void on_collision_enter(Collision info);
	virtual void on_collision_stay(Collision info);
	virtual void on_collision_exit(Collision info);
	virtual void on_trigger_enter(Collider info);
	virtual void on_trigger_stay(Collider info);
	virtual void on_trigger_exit(Collider info);

	virtual void set_world_position(Vector2f pos);
	virtual void set_world_rotation(float rot);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	void set_game_object(GameObject* go);
	virtual void set_active(bool active);
	GameObject* get_game_object();

	virtual std::vector<Editor::SerializedVar> get_editor_values();
	virtual void editor_update() {}

protected:

	bool active = true;
	GameObject* game_object;


};
}