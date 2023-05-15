#include "pch.h"
#include "core/Renderer.h"
#include "core/Input.h"
#include "ButtonComponent.h"
#include "GameObject.h"
#include "core/ResourceManager.h"
namespace bm98
{
using namespace core;

ButtonComponent::ButtonComponent()
{
	this->id = 0;
	this->button_state = ButtonState::BTN_IDLE;
	this->shape.setSize(sf::Vector2f(100, 50));
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_idle_color);
	
	this->font = &ResourceManager::Instance()->get_font("calibri-regular.ttf");

	this->text.setString("");
	this->text.setFont(*font);
	this->text.setFillColor(text_idle);
	this->text.setCharacterSize(30);

	this->text.setPosition(
		shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height /*/ 2.f*/
	);

	text_idle = sf::Color(255, 255, 255, 150);
	text_hover = sf::Color(255, 255, 255, 200);
	text_active = sf::Color(250, 250, 250, 50);

	idle_color = sf::Color(70, 70, 70, 200);
	hover_color = sf::Color(150, 150, 150, 200);
	active_color = sf::Color(20, 20, 20, 200);

	outline_idle_color = sf::Color(255, 255, 255, 200);
	outline_hover_color = sf::Color(255, 255, 255, 255);
	outline_active_color = sf::Color(20, 20, 20, 50);

	shape.setFillColor(idle_color);
	
	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(id + 10, false);
	
}
ButtonComponent::ButtonComponent(float width, float height, std::string text, 
	unsigned character_size, short unsigned id)
{
	this->id = id;
	this->button_state = ButtonState::BTN_IDLE;
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_idle_color);
	this->font = font;
	this->text.setString(text);
	this->text.setFont(*font);
	this->text.setFillColor(text_idle);
	this->text.setCharacterSize(character_size);

	this->text.setPosition(
		shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height /*/ 2.f*/
	);

	text_idle = sf::Color(255, 255, 255, 150);
	text_hover = sf::Color(255, 255, 255, 200);
	text_active = sf::Color(250, 250, 250, 50);

	idle_color = sf::Color(70, 70, 70, 200);
	hover_color = sf::Color(150, 150, 150, 200);
	active_color = sf::Color(20, 20, 20, 200);

	outline_idle_color = sf::Color(255, 255, 255, 200);
	outline_hover_color = sf::Color(255, 255, 255, 255);
	outline_active_color = sf::Color(20, 20, 20, 50);

	shape.setFillColor(idle_color);

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(id+10, false);

	Renderer::Instance()->add(Renderer::RenderObject(&shape, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer()));
	Renderer::Instance()->add(Renderer::RenderObject(&this->text, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer()));
}

ButtonComponent::~ButtonComponent()
{
	Renderer::Instance()->remove(&shape);
	Renderer::Instance()->remove(&text);
}

void ButtonComponent::init()
{
	this->shape.setPosition(game_object->get_world_position());
	Renderer::Instance()->add(Renderer::RenderObject(&shape, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer()));
	Renderer::Instance()->add(Renderer::RenderObject(&this->text, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer()));
}

void ButtonComponent::update()
{
	if (!active)
		return;
	if (shape.getGlobalBounds().contains(Input::Instance()->mouse_position(get_view())) &&
		Input::Instance()->get_mouse_down(Input::Mouse::LEFT))
	{
		button_state = ButtonState::BTN_PRESSED;
	}
	else if (shape.getGlobalBounds().contains(Input::Instance()->mouse_position(get_view())))
	{
		button_state = ButtonState::BTN_HOVERED;
	}
	else
	{
		button_state = ButtonState::BTN_IDLE;
	}

	switch (button_state)
	{
	case ButtonState::BTN_IDLE:
		shape.setFillColor(idle_color);
		shape.setOutlineColor(outline_idle_color);
		text.setFillColor(text_idle);
		break;
	case ButtonState::BTN_HOVERED:
		shape.setFillColor(hover_color);
		shape.setOutlineColor(outline_hover_color);
		text.setFillColor(text_hover);
		break;
	case ButtonState::BTN_PRESSED:
		shape.setFillColor(active_color);
		shape.setOutlineColor(outline_active_color);
		text.setFillColor(text_active);
		break;
	default:
		shape.setFillColor(sf::Color::Red);

	}
}

void ButtonComponent::add_to_buffer(sf::View* view)
{
	set_view(view);
}

Json::Value ButtonComponent::serialize_json()
{
	Json::Value obj;

	obj[RemoveNamespace(typeid(IRenderable).name())] = IRenderable::serialize_json();

	obj["width"] = get_width();
	obj["height"] = get_height();
	obj["id"] = id;
	std::string x = text.getString();
	obj["text"] = x;

	return obj;
}

void ButtonComponent::unserialize_json(Json::Value obj)
{
	IRenderable::unserialize_json(obj[RemoveNamespace(typeid(IRenderable).name())]);
	shape.setSize(sf::Vector2f(obj["width"].asFloat(), obj["height"].asFloat()));
	id = static_cast<short>(obj["id"].asInt64());
	text.setString(obj["text"].asString());
}

void ButtonComponent::set_active(bool active)
{
	Component::set_active(active);
	set_render(active);
}

const bool ButtonComponent::is_pressed() const
{
	return button_state == ButtonState::BTN_PRESSED;
}

bool ButtonComponent::mouse_in_bounds()
{
	return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(
		Input::Instance()->mouse_position( get_view() )));
}

const std::string ButtonComponent::get_text() const
{
	return text.getString();
}

const short unsigned& ButtonComponent::get_id() const
{
	return id;
}

const float ButtonComponent::get_height() const
{
	return shape.getGlobalBounds().height;
}

const float ButtonComponent::get_width() const
{
	return shape.getGlobalBounds().width;
}

void ButtonComponent::set_pressed()
{
	button_state = ButtonState::BTN_PRESSED;
}

void ButtonComponent::set_text(const std::string text)
{
	this->text.setString(text);
	this->text.setPosition(
		shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height /*/ 2.f*/
	);
}

void ButtonComponent::set_id(const short unsigned id)
{
	this->id = id;
}

void ButtonComponent::init_font()
{
}

}