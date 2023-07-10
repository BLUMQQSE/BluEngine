#pragma once
#include "globals.h"
#include "core/Renderer.h"
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
		std::vector<std::string> extra_data = std::vector<std::string>(), bool modifiable = true)
	{
		this->name = name;
		this->variable = variable;
		this->extra_data = extra_data;
		this->type = type;
		this->modifiable_from_editor = modifiable;
	}
	
	SerializedVar(std::string name, void* variable, Var::Type type, bool modifiable)
	{
		this->name = name;
		this->variable = variable;
		this->extra_data = {};
		this->type = type;
		this->modifiable_from_editor = modifiable;
	}

	std::string name;
	void* variable;
	std::vector<std::string> extra_data;
	Var::Type type;
	bool modifiable_from_editor;

};
}
class Component : public IData, public std::enable_shared_from_this<Component>
{

public:

	std::shared_ptr<Component> self()
	{
		return shared_from_this();
	}

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

	virtual void on_draw_gizmos();
	virtual void on_draw_gizmos_selected();
	/// <summary>
	/// Called on gameobject which has collided with another collider for first
	/// fixed update.
	/// </summary>
	virtual void on_collision_enter(Collision info);
	/// <summary>
	/// Called on gameobject which has maintained contact with another collider since
	/// previous fixed_update.
	/// </summary>
	virtual void on_collision_stay(Collision info);
	/// <summary>
	/// Called on gameobject which has lost contact with a prior collider.
	/// </summary>
	virtual void on_collision_exit(Collision info);
	/// <summary>
	/// Called on object containing a trigger which had a non-trigger collider enter for 
	/// first fixed_update.
	/// </summary>
	virtual void on_trigger_enter(Collider info);
	/// <summary>
	/// Called on gameobject containing a trigger which has maintained contact 
	/// with another collider since previous fixed_update.
	/// </summary>
	virtual void on_trigger_stay(Collider info);
	/// <summary>
	/// Called on gameobject containing a trigger which has lost contact with a prior collider.
	/// </summary>
	virtual void on_trigger_exit(Collider info);
	/// <summary>
	/// Called on object containing a trigger or nontrigger which had entered trigger collider
	/// for first fixed_update.
	/// </summary>
	virtual void on_other_trigger_enter(Collider info);
	/// <summary>
	/// Called on gameobject containing a trigger or nontrigger which has maintained contact 
	/// with another trigger since previous fixed_update.
	/// </summary>
	virtual void on_other_trigger_stay(Collider info);
	/// <summary>
	/// Called on gameobject containing a trigger or nontrigger which has lost contact
	///  with a prior trigger.
	/// </summary>
	virtual void on_other_trigger_exit(Collider info);

	virtual void set_world_position(Vector2f pos);
	virtual void set_world_rotation(float rot);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	void set_game_object(std::shared_ptr<GameObject> go);
	virtual void set_active(bool active);
	std::shared_ptr<GameObject> get_game_object();

	virtual std::vector<Editor::SerializedVar> get_editor_values();
	virtual void editor_update() {}


protected:

	bool active = true;
	std::shared_ptr<GameObject> game_object;


};
}