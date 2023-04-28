#include "pch.h"
#include "DropDownComponent.h"
#include "core/ResourceManager.h"
#include "GameObject.h"
#include "core/SceneManager.h"
#include "core/Input.h"
namespace bm98
{

using namespace core;

DropDownComponent::DropDownComponent()
{
}

DropDownComponent::DropDownComponent(float width, float height, std::vector<std::string> list,
	int char_size, unsigned default_index)
{


	this->width = width;
	this->height = height;
	this->text_list = list;
	this->char_size = char_size;
	this->default_index = default_index;
}

DropDownComponent::~DropDownComponent()
{

}

void DropDownComponent::init()
{

	GameObject* new_child = new GameObject();
	//game_object->add_child(new_child);
	new_child->set_parent(game_object);

	new_child->add_component<ButtonComponent>(width, height, text_list[0], char_size);
	active_selection = &new_child->get_component<ButtonComponent>();

	for (size_t i = 0; i < text_list.size(); i++)
	{
		GameObject* new_child = new GameObject();
		//game_object->add_child(new_child);
		new_child->set_parent(game_object);

		new_child->set_local_position(Vector2f(0, (i + 1) * height));
		new_child->add_component<ButtonComponent>(width, height, text_list[i], char_size, i);
		new_child->set_active(false);

		this->list.push_back(&new_child->get_component<ButtonComponent>());
		this->list[i]->set_render(false);

		SceneManager::Instance()->instantiate_gameobject(new_child);
	}

	active_selection->set_id(default_index);
}

void DropDownComponent::update()
{
	return;
	if (active_selection->is_pressed())
	{
		toggle_list(!show_list);
	}

	if (show_list)
	{
		for (auto& i : list)
		{
			if (i->is_pressed())
			{
				toggle_list(false);
				selection_change = true;
				active_selection->set_text(i->get_text());
				active_selection->set_id(i->get_id());
				return;
			}
		}
		if (Input::Instance()->get_mouse_down(Input::Mouse::LEFT) && !active_selection->is_pressed())
			toggle_list(false);
	}

}

void DropDownComponent::add_to_buffer(sf::View* view)
{
	return;
	active_selection->add_to_buffer(view);

	active_selection->set_view(view);
	if (show_list)
		for (auto& b : list)
			b->set_view(view);
}

Json::Value DropDownComponent::serialize_json()
{
	return Json::Value();
}

void DropDownComponent::unserialize_json(Json::Value obj)
{
}

void DropDownComponent::set_active(bool active)
{
	Component::set_active(active);
}

void DropDownComponent::toggle_list(bool toggle)
{
	show_list = toggle;
	for (auto& l : list)
	{
		l->set_active(toggle);
	}
}

bool DropDownComponent::changed_selection()
{
	if (selection_change)
	{
		selection_change = false;
		return true;
	}
	return false;
}

const bool DropDownComponent::mouse_in_bounds()
{
	for (auto& f : list)
	{
		if (f->mouse_in_bounds())
			return true;
	}
	return false;
}

void DropDownComponent::set_selected_index(int index)
{
	active_selection->set_text(list[index]->get_text());
	active_selection->set_id(list[index]->get_id());
}

const short unsigned& DropDownComponent::get_selected_index() const
{
	return active_selection->get_id();
}


}