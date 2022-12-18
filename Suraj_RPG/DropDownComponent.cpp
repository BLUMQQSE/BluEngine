#include "pch.h"
#include "DropDownComponent.h"
#include "ResourceManager.h"
namespace bm98
{

using namespace core;

DropDownComponent::DropDownComponent()
{
}

DropDownComponent::DropDownComponent(float width, float height, std::vector<std::string> list,
	unsigned default_index, int char_size)
{
	
}

DropDownComponent::~DropDownComponent()
{

}

void DropDownComponent::init()
{
}

void DropDownComponent::update()
{
}

void DropDownComponent::add_to_buffer(sf::View* view)
{
}

Json::Value DropDownComponent::serialize_json()
{
	return Json::Value();
}

void DropDownComponent::unserialize_json(Json::Value obj)
{
}

void DropDownComponent::toggle_list(bool toggle)
{
}

bool DropDownComponent::changed_selection()
{
	return false;
}

const bool DropDownComponent::mouse_in_bounds()
{
	return false;
}

void DropDownComponent::set_selected_index(int index)
{
}

const short unsigned& DropDownComponent::get_selected_index() const
{
	return 0;
}


}