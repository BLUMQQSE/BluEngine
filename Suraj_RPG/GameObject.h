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
using ComponentArray = std::array<std::weak_ptr<Component>, max_components>;

class GameObject : public IData, public IObject, public std::enable_shared_from_this<GameObject>
{

public:

	
	std::shared_ptr<GameObject> self()
	{
		return shared_from_this();
	}

	struct Info
	{
		unsigned long unique_id;
		std::string name;
		Tags::Tag tag;
		PhysicsNS::Layer layer;

		Json::Value serialize_json()
		{
			Json::Value obj;

			obj["name"] = name;
			obj["tag"] = Tags::ToString(tag);
			obj["physics-layer"] = PhysicsNS::ToString(layer);
			obj["unique-id"] = (uint64_t)unique_id;


			return obj;
		}

		void unserialize_json(Json::Value obj)
		{
			name = obj["name"].asString();
			tag = Tags::ToTag(obj["tag"].asString());
			layer = PhysicsNS::ToLayer(obj["physics-layer"].asString());
			unique_id = obj["unique-id"].asUInt64();
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
	const Vector2f& get_world_position() const { return position; }

	/// <returns>Position of gameobject relative to its parent. If no parent
	///exists, the local position is 0.</returns>
	const Vector2f& get_local_position() const { return local_position; }

	void set_world_position(const Vector2f pos)
	{
		if (parent.lock())
			local_position = pos - parent.lock()->get_world_position();

		Vector2f change = pos - position;
		position = pos;

		for (std::size_t i = 0; i < components.size(); i++)
			components[i]->set_world_position(pos);

		for (std::size_t i = 0; i < children.size(); i++)
			children[i].lock()->set_world_position(children[i].lock()->get_world_position()
				+ change);

	}

	void set_local_position(const Vector2f pos)
	{
		if (!parent.lock())
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
			children[i].lock()->set_world_position(children[i].lock()->get_world_position()
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
		if (parent.lock())
			local_rotation = parent.lock()->rotation - rotation;
		else
			local_position = Vector2f::Zero();
		for (std::size_t i = 0; i < children.size(); i++)
			children[i].lock()->set_world_rotation(children[i].lock()->get_local_rotation() + rot);
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

	virtual void on_draw_gizmos() override;
	virtual void on_draw_gizmos_selected() override;

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
	void set_parent(std::shared_ptr<GameObject> parent);
	void remove_child(std::shared_ptr<GameObject> child);

	const size_t get_unique_runtime_id() const;
	Info& get_info();
	Vector2f get_visual_center();
	Vector2f get_physical_center();
	std::weak_ptr<GameObject> get_parent();
	/// <summary>
	/// Returns all parents to root ancestor, as well as all children.
	/// </summary>
	/// <returns></returns>
	std::vector<std::weak_ptr<GameObject>> get_all_relatives();
	/// <summary>
	/// Returns the highest parent of a game object.
	/// </summary>
	std::weak_ptr<GameObject> get_greatest_ancestor();
	std::vector<std::weak_ptr<Component>> get_components();
	std::vector<std::weak_ptr<GameObject>> get_children();
	/// <summary>
	/// Returns all children, grandchildren, great-grandchildren...
	/// </summary>
	/// <returns></returns>
	std::vector<std::weak_ptr<GameObject>> get_all_posterity();
	const bool check_for_child(std::shared_ptr<GameObject> game_object) const;
	const bool check_for_child(std::string name) const;
	std::weak_ptr<GameObject> get_child(std::string name);
	const bool is_initialized() const;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;


#pragma region MODIFYING

	template <typename T, typename... TArgs> std::weak_ptr<T> add_component(TArgs&&...mArgs)
	{
		//if allready has component, ignore adding it again
		
		if (has_component<T>())
			return get_component<T>();
			
			//return get_component<T>();

		T* p(new T(std::forward<TArgs>(mArgs)...));
		//std::shared_ptr<T> c = std::make_shared<T>(std::forward<TArgs>((mArgs)...));
		std::shared_ptr<T>c(p);

		c->set_game_object(self());
		components.push_back(c);

		component_array[get_component_type_id<T>()] = c;
		component_bitset[get_component_type_id<T>()] = true;
		
		EventSystem::Instance()->push_event(EventID::GAMEOBJECT_COMPONENT_ADDED_FLAG,
			static_cast<void*>(c.get()), Caller(Caller::Name::GAMEOBJECT, (void*)this));

		return c;
	}

	template <typename T> void remove_component()
	{
		if (has_component<T>())
		{
			std::shared_ptr<Component> component = get_component<T>().lock();
			auto i = std::find_if(components.begin(), components.end(), 
								  [component]
									(std::shared_ptr<Component> const& i)
										{ return i.get() == component.get(); }
									);

			if (i != components.end())
			{
				components_to_remove.push_back(component);
				component_bitset[get_component_type_id<T>()] = false;

				// issue with this?
				EventSystem::Instance()->push_event(EventID::GAMEOBJECT_COMPONENT_REMOVED_FLAG,
													static_cast<void*>(get_component<T>().lock().get()), Caller(Caller::Name::GAMEOBJECT, (void*)this));
			}
		}
	}

	template <typename T> void remove_component_of_type()
	{
		for (auto it = components.begin(); it != components.end(); ++it)
		{
			if (static_cast<T*>(*it))
			{
				components_to_remove.push_back(*it);

				// Worth noting this is an issue... T will refer to the type rather than the specific component:
				// Ex: will set ColliderComponent false rather than desired BoxColliderComponent false
				// Possible fix, send all components their type when added
				component_bitset[get_component_type_id<T>()] = false;

				EventSystem::Instance()->push_event(EventID::GAMEOBJECT_COMPONENT_REMOVED_FLAG,
													static_cast<void*>(*it), Caller(Caller::Name::GAMEOBJECT, (void*)this));
				
				return;
			}
		}
	}

	template <typename T> bool has_component() const
	{
		return component_bitset[get_component_type_id<T>()];
	}

	template <typename T> bool has_component_of_type() const
	{
		for (std::size_t i = 0; i < components.size(); i++)
		{
			if (dynamic_cast<T*>(components[i].get()))
				return true;
		}
		return false;
	}

	template<typename T> bool has_component_with_interface() const
	{
		return has_component_of_type<T>();
	}

	template <typename T> std::weak_ptr<T> get_component() const
	{
		//auto ptr(component_array[get_component_type_id<T>()]);
		//return static_cast<T*>(ptr);

		//return std::weak_ptr<T>();
		return std::static_pointer_cast<T>(component_array[get_component_type_id<T>()].lock());
		
	}

	/// <summary>
	/// Used for checking if a component is derived from a non-component interface
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template <typename T> T* get_component_with_interface()
	{
		for (std::size_t i = 0; i < components.size(); i++)
		{
			// issue lies here in converting from component shared ptr to idamageable weak_ptr
			if (dynamic_cast<T*>(components[i].get()))
				return dynamic_cast<T*>(components[i].get());
		}

		return nullptr;
	}

	template <typename T> std::weak_ptr<T> get_component_of_type() 
	{
		for (std::size_t i = 0; i < components.size(); i++)
		{
			// issue lies here in converting from component shared ptr to idamageable weak_ptr
			if (dynamic_cast<T*>(components[i].get()))
				return std::static_pointer_cast<T>(components[i]);
		}

		return std::weak_ptr<T>();
	}

	template <typename T> std::vector<std::weak_ptr<T>> get_components_of_type()
	{
		std::vector<std::weak_ptr<T>> components_of_type;
		for (std::size_t i = 0; i < components.size(); i++)
		{
			if (static_cast<T*>(components[i].get()))
				components_of_type.push_back(std::static_pointer_cast<T>(components[i]));
		}
		
		return components_of_type;

	}

#pragma endregion

	/// <summary>
	/// Adds component of type "name" to the gameobject.
	/// </summary>
	/// <returns></returns>
	std::weak_ptr<Component> add_component_by_name(std::string name, Json::Value data = Json::Value(),
									 std::unordered_map<std::string, Json::Value>* data_map = nullptr);

	/// <summary>
	/// Adds component of type "name" to the component_to_remove vector.
	/// </summary>
	void remove_component_by_name(std::string name);
	/// <summary>
	/// Removes all components from the gameobject which are in the component_to_remove vector.
	/// NOTE: Should only be used by Editor.
	/// </summary>
	void handle_removed_components();

protected:
	bool active = true;
	std::weak_ptr<GameObject> parent;
	std::vector<std::weak_ptr<GameObject>> children;

	//std::vector<std::unique_ptr<Component>> components;
	std::vector<std::shared_ptr<Component>> components;
	std::vector<std::shared_ptr<Component>> components_to_remove;
private:
	size_t unique_runtime_id;

	bool initialized = false;

	ComponentArray component_array;
	ComponentBitSet component_bitset;

	void init_components();
	void awake_components();
	void start_components();

	void add_child(std::shared_ptr<GameObject> child);

	static size_t get_unique_id()
	{
		current_id++;
		return current_id;
	}
};

}