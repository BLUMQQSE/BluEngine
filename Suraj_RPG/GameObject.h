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
	};
	struct Transform
	{
		Vector2f position;
		// position relative to parent
		Vector2f local_position;
		Vector2f rotation;
		Vector2f scale;
	};

	GameObject();
	virtual ~GameObject();


	Info info;
	//this transform thing may be a bad idea
	//difficult to ensure insync with sprite position at all times
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

	Renderer::RenderObject& get_render_object();
	const bool& is_active();
	void set_active(bool& active);

	void set_sprite_texture(sf::Texture& texture);
	void set_parent(GameObject* parent);
	void add_child(GameObject* child);
	void remove_child(GameObject* child);

	virtual void set_position(const float x, const float y);

	const size_t get_unique_runtime_id() const;
	const Info& get_info() const;
	const Transform& get_transform() const;
	GameObject* get_parent();
	std::set<GameObject*> get_children();
	const bool& check_for_child(GameObject* game_object) const;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	template <typename T> bool has_component() const
	{
		return component_bitset[get_component_type_id<T>()];
	}

	template <typename T, typename... TArgs> T& add_component(TArgs&&...mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		//c->game_object = this;
		c->set_game_object(this);
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		component_array[get_component_type_id<T>()] = c;
		component_bitset[get_component_type_id<T>()] = true;
		return *c;
	}

	template <typename T> void remove_component(T* comp)
	{
		if (component_bitset[get_component_type_id<T>()])
		{

			auto itr = std::find_if(std::begin(components),
				std::end(components),
				[comp](auto& element) { return element.get() == comp; });

			components.erase(itr);

			component_array[get_component_type_id<T>()] = nullptr;
			component_bitset[get_component_type_id<T>()] = false;
		}
	}

	template <typename T> T& get_component() const
	{
		auto ptr(component_array[get_component_type_id<T>()]);
		return *static_cast<T*>(ptr);
	}

protected:
	bool active;
	GameObject* parent;
	std::set<GameObject*> children;

	Renderer::RenderObject* render_object;
	sf::Sprite sprite;
	sf::Texture texture_sheet;

	std::vector<std::unique_ptr<Component>> components;

private:
	size_t unique_runtime_id;

	virtual void init_variables();
	void init_components();
	void awake_components();
	void start_components();

	ComponentArray component_array;
	ComponentBitSet component_bitset;

	static size_t get_unique_id()
	{
		current_id++;
		return current_id;
	}
};

}