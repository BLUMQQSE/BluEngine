#include "pch.h"
#include "InventoryWindow.h"
#include "Inventory.h"
#include "GameObject.h"
#include "ResourceManager.h"

namespace bm98
{


void InventoryWindow::init()
{
	inventory = &game_object->get_component<Inventory>();
	int size = inventory->get_size();

	if (size == 18)
	{
		x_slots = 6;
		y_slots = 3;
	}	
	else if (size == 10)
	{
		x_slots = 2;
		y_slots = 5;
	}
	else
	{
		x_slots = size * .5f;
		y_slots = 2;
	}

	panel = new GUI::Panel(0, 0, (x_slots) * 50 + 10, (y_slots * 50) + 10);

	EventSystem::Instance()->subscribe(EventID::INTERACTION_INVENTORY_UPDATED, this);
	create_inventory_window();

	set_render(false);
}
void InventoryWindow::set_position(Vector2f pos)
{
	panel->set_position(pos.x, pos.y);
}

void InventoryWindow::move(Vector2f change)
{
	Vector2f position = panel->get_position();
	position += change;
	panel->set_position(position.x, position.y);
}

void InventoryWindow::refresh_window()
{
	clear_inventory_window();
	create_inventory_window();
}

void InventoryWindow::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
	case EventID::INTERACTION_INVENTORY_UPDATED:
		
		if (static_cast<Inventory*>(event->get_caller().pointer) == inventory)
			refresh_window();
		break;
	default:
		break;
	}
}

void InventoryWindow::create_inventory_window()
{
	int i = 0;
	for (int yy = 0; yy < y_slots; yy++)
	{
		for (int xx = 0; xx < x_slots; xx++)
		{
			panel->add_element("Slot" + std::to_string(i), new GUI::Panel(10 + (xx * 50), 10 + (yy * 50), 40, 40, sf::Color::Black));
			panel->add_element("Image" + std::to_string(i), new GUI::Panel(15 + (xx * 50), 15 + (yy * 50), 30, 30, sf::Color::Transparent));
			panel->add_element("Count"+std::to_string(i), new GUI::Label(35 + (xx*50), 30 + (yy*50), 14, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), "", sf::Color::White));

			slots.push_back(panel->get_panel("Slot"+std::to_string(i)));
			images.push_back(panel->get_panel("Image" + std::to_string(i)));
			counts.push_back(dynamic_cast<GUI::Label*>(panel->get_element("Count"+std::to_string(i))));

			if (inventory->get_item(i))
			{
				images[i]->set_fill_color(sf::Color::White);
				images[i]->apply_texture(
					inventory->get_item(i)->get_texture(), 
					inventory->get_item(i)->get_rect()
				);
				if(inventory->get_capacity(i) > 1)
					counts[i]->set_text(std::to_string(inventory->get_capacity(i)));
			}
			i++;
		}
	}
	panel->set_render(panel->get_render());
}

void InventoryWindow::clear_inventory_window()
{
	panel->clear();

	slots.clear();
	images.clear();
	counts.clear();
}

std::vector<Editor::SerializedVar> InventoryWindow::get_editor_values()
{
	return std::vector<Editor::SerializedVar>();
}


}