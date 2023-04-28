#pragma once
#include "core/Debug.h"
#include "IData.h"
#include "IObject.h"
#include "Math.h"
#include "Component.h"
namespace bm98
{

class Collision;
class Collider;

class Component;

static size_t current_id = 0;

using ComponentID = std::size_t;
static ComponentID last_id = 0;
inline ComponentID get_component_type_id()
{
	return last_id++;
}
template <typename T> inline ComponentID get_component_type_id() noexcept
{
	static ComponentID type_id = get_component_type_id();
	return type_id;
}
constexpr std::size_t max_components = 150;
using ComponentBitSet = std::bitset<max_components>;
using ComponentArray = std::array<Component*, max_components>;

class GameObject : public IData, public IObject
{

public:

	struct Info
	{
		unsigned int id;
		std::string name;
		Tags::Tag tag;
		PhysicsNS::Layer layer;

		Json::Value serialize_json()
		{
			Json::Value obj;

			obj["name"] = name;
			obj["tag"] = Tags::ToString(tag);
			obj["physics-layer"] = PhysicsNS::ToString(layer);

			return obj;
		}

		void unserialize_json(Json::Value obj)
		{
			name = obj["name"].asString();
			tag = Tags::ToTag(obj["tag"].asString());
			layer = PhysicsNS::ToLayer(obj["physics-layer"].asString());
		}

	};

	GameObject();
	virtual ~GameObject();

	
	Info info;
	
#pragma region Transform

	Vector2f position;
	// position relative to parent
	Vector2f local_position;
	float rotation;
	float local_rotation;

#pragma region Position

	/// <returns>Position of gameobject relative to the game world.</returns>
	const Vector2f get_world_position() const { return position; }

	/// <returns>Position of gameobject relative to its parent. If no parent
	///exists, the local position is 0.</returns>
	const Vector2f get_local_position() const { return local_position; }
	
	/*
	void set_world_position(const Vector2f pos)
	{
		position = pos;
		if (parent)
			local_position = parent->position - position;
		else
			local_position = Vector2f::zero();
		for (std::size_t i = 0; i < children.size(); i++)
			children[i]->set_world_position(children[i]->get_local_position() + pos);
		for (std::size_t i = 0; i < components.size(); i++)
			components[i]->set_world_position(pos);
	}

	void set_local_position(const Vector2f pos)
	{
		Vector2f change = pos - local_position;
		set_world_position(position + change);
	}

	void move(const Vector2f offset)
	{
		set_world_position(position + offset);
	}
	*/

	void set_world_position(const Vector2f pos)
	{
		if (parent)
			local_position = pos - parent->get_world_position();

		Vector2f change = pos - position;
		position = pos;

		for (std::size_t i = 0; i < components.size(); i++)
			components[i]->set_world_position(pos);

		for (std::size_t i = 0; i < children.size(); i++)
			children[i]->set_world_position(children[i]->get_world_position()
				+ change);

	}

	void set_local_position(const Vector2f pos)
	{
		if (!parent)
		{
			local_position = position;
			return;
		}

		Vector2f change = pos - get_local_position();

		local_position = pos;
		position += pos;

		for (std::size_t i = 0; i < components.size(); i++)
			components[i]->set_world_position(pos);

		for (std::size_t i = 0; i < children.size(); i++)
			children[i]->set_world_position(children[i]->get_world_position()
				+ change);

	}

	void move(const Vector2f offset)
	{
		set_world_position(position + offset);
	}


public:

#pragma endregion

#pragma region Rotation

	/// <returns>Position of gameobject relative to the game world.</returns>
	const float get_world_rotation() const { return rotation; }
	/// <returns>Position of gameobject relative to its parent. If no parent
	///exists, the local position is 0.</returns>
	const float get_local_rotation() const { return local_rotation; }
	void set_world_rotation(const float rot)
	{
		rotation = rot;
		if (parent)
			local_rotation = parent->rotation - rotation;
		else
			local_position = Vector2f::Zero();
		for (std::size_t i = 0; i < children.size(); i++)
			children[i]->set_world_rotation(children[i]->get_local_rotation() + rot);
		for (std::size_t i = 0; i < components.size(); i++)
			components[i]->set_world_rotation(rot);
	}
	void set_local_rotation(const float rot)
	{
		float change = rot - local_rotation;
		set_world_rotation(change);
	}
	void rotate(const float rot)
	{
		set_world_rotation(rotation + rot);
	}
	

#pragma endregion

#pragma endregion


	virtual void init();
	virtual void awake();
	virtual void start();

	virtual void on_destroy();

	virtual void update() override;
	virtual void late_update() override;
	virtual void fixed_update() override;

	virtual void on_collision_enter(Collision info);
	virtual void on_collision_stay(Collision info);
	virtual void on_collision_exit(Collision info);
	virtual void on_trigger_enter(Collider info);
	virtual void on_trigger_stay(Collider info);
	virtual void on_trigger_exit(Collider info);

	virtual void add_to_buffer(sf::View* view = nullptr) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values();
	virtual void editor_update();

	const bool& is_active();
	void set_active(bool active);
	void set_render(bool render);

	/// <summary>
	/// Sets the parent of this gameobject to parent. Additionally removes from children of prior parent if one
	/// existed.
	/// </summary>
	void set_parent(GameObject* parent);
	void remove_child(GameObject* child);

	const size_t get_unique_runtime_id() const;
	Info& get_info();
	Vector2f get_center();
	GameObject* get_parent();
	/// <summary>
	/// Returns all parents to root ancestor, as well as all children.
	/// </summary>
	/// <returns></returns>
	std::vector<GameObject*> get_all_relatives();
	/// <summary>
	/// Returns the highest parent of a game object.
	/// </summary>
	GameObject* get_greatest_ancestor();
	std::vector<Component*> get_components();
	std::vector<GameObject*> get_children();
	/// <summary>
	/// Returns all children, grandchildren, great-grandchildren...
	/// </summary>
	/// <returns></returns>
	std::vector<GameObject*> get_all_posterity();
	const bool check_for_child(GameObject* game_object) const;
	const bool check_for_child(std::string name) const;
	GameObject* get_child(std::string name);
	const bool is_initialized() const;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;


	template <typename T> bool has_component() const
	{
		
		return component_bitset[get_component_type_id<T>()];
	}

	template <typename T> bool has_component_of_type() const
	{
		for (std::size_t i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i]))
				return true;
		}
		return false;
	}

	template <typename T, typename... TArgs> T& add_component(TArgs&&...mArgs)
	{
		//if allready has component, ignore adding it again
		if (component_bitset[get_component_type_id<T>()] == true)
			return *dynamic_cast<T*>(component_array[get_component_type_id<T>()]);

		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->set_game_object(this);
		components.push_back(c);

		component_array[get_component_type_id<T>()] = c;
		component_bitset[get_component_type_id<T>()] = true;
		
		EventSystem::Instance()->push_event(EventID::GAMEOBJECT_COMPONENT_ADDED,
			static_cast<void*>(c), Caller(Caller::Name::GAMEOBJECT, (void*)this));

		return *c;
	}

	template <typename T> void remove_component()
	{
		if (component_bitset[get_component_type_id<T>()])
		{
			std::vector<Component*>::iterator iter = std::find(components.begin(), components.end(), &get_component<T>());
			if (iter != components.end())
			{
				components_to_remove.push_back(&get_component<T>());

				component_array[get_component_type_id<T>()] = nullptr;
				component_bitset[get_component_type_id<T>()] = false;

				EventSystem::Instance()->push_event(EventID::GAMEOBJECT_COMPONENT_REMOVED,
					static_cast<void*>(&get_component<T>()), Caller(Caller::Name::GAMEOBJECT, (void*)this));
			}
		}
	}

	template <typename T> void remove_component_of_type()
	{
		for (std::size_t i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i]))
			{
				T* g = get_component_of_type<T>();
				std::vector<Component*>::iterator iter = std::find(components.begin(), components.end(), g);
				if (iter != components.end())
				{
					components_to_remove.push_back(g);

					component_array[get_component_type_id<T>()] = nullptr;
					component_bitset[get_component_type_id<T>()] = false;

					EventSystem::Instance()->push_event(EventID::GAMEOBJECT_COMPONENT_REMOVED,
						static_cast<void*>(g), Caller(Caller::Name::GAMEOBJECT, (void*)this));
				}
			}
		}
		//return nullptr;
	}

	template <typename T> T& get_component() const
	{
		auto ptr(component_array[get_component_type_id<T>()]);
		return *static_cast<T*>(ptr);
	}

	template <typename T> T* get_component_of_type() 
	{
		for (std::size_t i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i]))
				return dynamic_cast<T*>(components[i]);
		}
		return nullptr;
	}

	template <typename T> std::vector<T*> get_components_of_type()
	{
		std::vector<T*> components_of_type;
		for (std::size_t i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i]))
				components_of_type.push_back(dynamic_cast<T*>(components[i]));
		}
		
		return components_of_type;

	}

	Component* editor_add_component(std::string c);
	void editor_remove_component(std::string c);
	void handle_removed_components();

protected:
	bool active = true;
	GameObject* parent;
	std::vector<GameObject*> children;

	//std::vector<std::unique_ptr<Component>> components;
	std::vector<Component*> components;
	std::vector<Component*> components_to_remove;
private:
	size_t unique_runtime_id;

	bool initialized = false;

	ComponentArray component_array;
	ComponentBitSet component_bitset;

	virtual void init_variables();
	void init_components();
	void awake_components();
	void start_components();

	void add_child(GameObject* child);

	static size_t get_unique_id()
	{
		current_id++;
		return current_id;
	}
};

}