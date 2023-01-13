#pragma once
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

class GameObject : IData, public IObject
{
public:

	struct Info
	{
		unsigned int id;
		std::string name;
		Tag tag;
		Layer layer;

		Json::Value serialize_json()
		{
			Json::Value obj;

			obj["name"] = name;
			obj["tag"] = Global::tag_to_string(tag);
			obj["physics-layer"] = Global::physics_layer_to_string(layer);

			return obj;
		}

		void unserialize_json(Json::Value obj)
		{
			name = obj["name"].asString();
			tag = Global::string_to_tag(obj["tag"].asString());
			layer = Global::string_to_physics_layer(obj["physics-layer"].asString());
		}

	};
	struct Transform
	{
		Vector2f position;
		// position relative to parent
		Vector2f local_position;
		Vector2f rotation;
		Vector2f scale;

		Json::Value serialize_json()
		{
			Json::Value obj;

			obj["position"] = position.serialize_json();
			obj["local-position"] = local_position.serialize_json();
			obj["rotation"] = rotation.serialize_json();
			obj["scale"] = scale.serialize_json();

			return obj;
		}

		void unserialize_json(Json::Value obj)
		{
			position.unserialize_json(obj["position"]);
			local_position.unserialize_json(obj["local-position"]);
			rotation.unserialize_json(obj["rotation"]);
			scale.unserialize_json(obj["scale"]);
		}

	};

	GameObject();
	virtual ~GameObject();


	Info info;
	Transform transform;

	virtual void init();
	virtual void awake();
	virtual void start();

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

	const bool& is_active();
	void set_active(bool active);
	void set_render(bool render);

	void set_parent(GameObject* parent);
	void add_child(GameObject* child);
	void remove_child(GameObject* child);

	virtual void set_position(const float x, const float y);
	virtual void set_local_position(const float x, const float y);

	const size_t get_unique_runtime_id() const;
	const Info& get_info() const;
	const Transform& get_transform() const;
	const sf::Vector2f get_center() const;
	GameObject* get_parent();
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

	template <typename T, typename... TArgs> void add_component(TArgs&&...mArgs)
	{
		//if allready has component, ignore adding it again
		if (component_bitset[get_component_type_id<T>()] == true)
			return;

		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->set_game_object(this);
		components.push_back(c);

		component_array[get_component_type_id<T>()] = c;
		component_bitset[get_component_type_id<T>()] = true;

	}

	template <typename T> void remove_component(T* comp)
	{
		if (component_bitset[get_component_type_id<T>()])
		{
			std::vector<Component*>::iterator iter = std::find(components.begin(), components.end(), comp);
			if (iter != components.end())
			{
				components_to_remove.push_back(comp);

				component_array[get_component_type_id<T>()] = nullptr;
				component_bitset[get_component_type_id<T>()] = false;
			}
		}
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

	static size_t get_unique_id()
	{
		current_id++;
		return current_id;
	}
};

}