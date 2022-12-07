#include "pch.h"
#include "BoxColliderComponent.h"
#include "Renderer.h"
namespace bm98
{
using namespace core;

BoxColliderComponent::BoxColliderComponent(sf::Sprite& sprite, float offset_x,
	float offset_y, float width, float height, bool trigger,
	CollisionDetection collision_check_type)
	:sprite(sprite), offsetX(offset_x), offsetY(-offset_y), trigger(trigger), active(true),
	collision_check_type(collision_check_type)
{
	hitbox.setPosition(sprite.getPosition().x + offset_x, sprite.getPosition().y + offset_y);
	hitbox.setSize(sf::Vector2f(width, height));
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(2.f);
	hitbox.setOutlineColor(sf::Color::Blue);

}

BoxColliderComponent::~BoxColliderComponent()
{
}

bool BoxColliderComponent::check_intersect(const sf::FloatRect& frect)
{
	return hitbox.getGlobalBounds().intersects(frect);
}

bool BoxColliderComponent::check_outer_intersect(const sf::FloatRect& frect)
{
	return get_outer_bounds().intersects(frect);
}

const sf::FloatRect BoxColliderComponent::get_bounds() const
{
	return hitbox.getGlobalBounds();
}

const sf::FloatRect BoxColliderComponent::get_outer_bounds() const
{
	return sf::FloatRect(
		hitbox.getGlobalBounds().left - 1,
		hitbox.getGlobalBounds().top - 1,
		hitbox.getGlobalBounds().width + 2,
		hitbox.getGlobalBounds().height + 2
	);;
}

const bool BoxColliderComponent::is_trigger() const
{
	return trigger;
}

const bool BoxColliderComponent::is_active()
{
	return active;
}

void BoxColliderComponent::set_active(const bool& active)
{
	this->active = active;
}

Vector2f BoxColliderComponent::get_offset()
{
	return Vector2f(offsetX, offsetY);
}

void BoxColliderComponent::update()
{
	hitbox.setPosition(this->sprite.getPosition().x + offsetX,
		this->sprite.getPosition().y + offsetY);

}

void BoxColliderComponent::fixed_update()
{

}

void BoxColliderComponent::add_to_buffer(sf::View* view)
{
	Renderer::add(Renderer::RenderObject(&hitbox, SortingLayer::UI, 0, view));
}
}