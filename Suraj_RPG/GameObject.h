#pragma once
#include "IData.h"
#include "IObject.h"
namespace bm98
{
class Collision;
class Collider;

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
		sf::Vector2f position;
		// position relative to parent
		sf::Vector2f local_position;
		sf::Vector2f rotation;
		sf::Vector2f scale;
	};

	GameObject();
	virtual ~GameObject();

	virtual void init_object();

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

	void add_box_collider_component(sf::Sprite& sprite, float offset_x,
		float offset_y, float width, float height, bool trigger = false);
	void add_rigidbody_component(const float maxVelocity,
		const float acceleration, const float deceleration);
	void add_animation_component();
	void add_child_animation_component();
	void add_animated_sprite_component(sf::Texture& texture_sheet, sf::IntRect animation_rect,
		float animation_timer, int frame_width = UNIT_SIZE, int frame_height = UNIT_SIZE);
	void set_parent(GameObject* parent);
	void add_child(GameObject* child);

	virtual void set_position(const float x, const float y);

	const Info& get_info() const;
	const Transform& get_transform() const;
	GameObject* get_parent();
	std::set<GameObject*> get_children();
	const bool& check_for_child(GameObject* game_object) const;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;


	BoxColliderComponent* get_box_collider_component();
	RigidbodyComponent* get_rigidbody_component();
	AnimationComponent* get_animation_component();
	ChildAnimationComponent* get_child_animation_component();
	AnimatedSpriteComponent* get_animated_sprite_component();

protected:
	Info info;
	//this transform thing may be a bad idea
	//difficult to ensure insync with sprite position at all times
	Transform transform;
	bool active;

	GameObject* parent;
	std::set<GameObject*> children;

	Renderer::RenderObject* render_object;
	sf::Sprite sprite;
	sf::Texture texture_sheet;
	BoxColliderComponent* box_collider_component;
	RigidbodyComponent* rigidbody_component;
	AnimationComponent* animation_component;
	ChildAnimationComponent* child_animation_component;
	AnimatedSpriteComponent* animated_sprite_component;


private:
	virtual void init_variables();



};

}
