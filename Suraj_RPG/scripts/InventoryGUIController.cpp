#include "../pch.h"
#include "InventoryGUIController.h"
#include "../GameObject.h"
#include "Inventory.h"
#include "InventoryWindow.h"
#include "../core/Input.h"
#include "../core/ResourceManager.h"
#include "../core/SceneManager.h"

namespace bm98
{
InventoryGUIController::InventoryGUIController()
{
}
InventoryGUIController::~InventoryGUIController()
{
	Renderer::Instance()->remove_ui(&hand.image);
}
void InventoryGUIController::init()
{
	general_inventory = game_object->get_child("GeneralInventory").lock()->get_component<Inventory>().lock().get();
	general_window = game_object->get_child("GeneralInventory").lock()->get_component<InventoryWindow>().lock().get();

	combat_inventory = game_object->get_child("CombatInventory").lock()->get_component<Inventory>().lock().get();
	combat_window = game_object->get_child("CombatInventory").lock()->get_component<InventoryWindow>().lock().get();

	external_inventory = nullptr;
	external_window = nullptr;

}

void InventoryGUIController::awake()
{
	general_window->set_position(Vector2f(300, 300));
	combat_window->set_position(Vector2f(100, 300));

	general_window->set_render(false);
	combat_window->set_render(false);


	GUI::Panel* p = general_window->get_slots()[0];
	hand.init(p->get_position(), Vector2f(p->get_width(), p->get_height()));

	Renderer::Instance()->add_ui(Renderer::RenderObject(&hand.image, hand.render, hand.layer, hand.z_order));

}

void InventoryGUIController::update()
{
	if (!general_window->get_render())
		return;

	if (hand.item.item)
	{
		hand.image.set_position(Vector2f
		(
								core::Input::Instance()->mouse_position().x - hand.image_size.x,
								core::Input::Instance()->mouse_position().y - hand.image_size.y)
		);
	}

	if (core::Input::Instance()->get_mouse_down(core::Input::Mouse::LEFT))
	{
		//if (core::Input::Instance()->get_action("SHIFT"))
		//	handle_shift_left_click();
		//else
		handle_left_click();
	}
	if (core::Input::Instance()->get_mouse_down(core::Input::Mouse::RIGHT))
		handle_right_click();

}

void InventoryGUIController::toggle_inventory(InventoryNS::WindowToggle window_to_toggle)
{
	switch (window_to_toggle)
	{
		case bm98::InventoryNS::WindowToggle::GENERAL_INVENTORY:
			general_window->toggle_render();
			break;
		case bm98::InventoryNS::WindowToggle::COMBAT_INVENTORY:
			combat_window->toggle_render();
			break;
		case bm98::InventoryNS::WindowToggle::FULL_INVENTORY:
			if (general_window->get_render() ^ combat_window->get_render())
			{
				general_window->set_render(true);
				combat_window->set_render(true);
			}
			else if (general_window->get_render() || combat_window->get_render())
			{
				general_window->set_render(false);
				combat_window->set_render(false);

				hand.send_back();
			}
			else
			{
				general_window->set_render(true);
				combat_window->set_render(true);
			}
			break;
		case bm98::InventoryNS::WindowToggle::EXTERNAL_INVENTORY:
			if (external_window)
				external_window->toggle_render();
			break;
		case bm98::InventoryNS::WindowToggle::CLOSE_ALL:
			general_window->set_render(false);
			combat_window->set_render(false);
			if (external_window)
				external_window->set_render(false);

			hand.send_back();
			break;
		case bm98::InventoryNS::WindowToggle::OPEN_ALL:
			general_window->set_render(true);
			combat_window->set_render(true);
			if (external_window)
				external_window->set_render(true);
		default:
			break;
	}
}

void InventoryGUIController::set_external(InventoryWindow* inv)
{
	external_window = inv;
	if (!external_window)
	{
		external_inventory = nullptr;
	}
	else
	{
		external_inventory = inv->get_inventory();
		external_window->set_position(Vector2f(400, 50));
		external_window->refresh_window();
	}

}

void InventoryGUIController::remove_external()
{
	if (external_window)
	{
		external_window->set_position(Vector2f(0, 0));
		external_window->set_render(false);
		external_window->refresh_window();
	}
	external_inventory = nullptr;
	external_window = nullptr;
}

void InventoryGUIController::handle_left_click()
{
	InventoryWindow* mouse_window = nullptr;
	Inventory* mouse_inven = nullptr;

	int slot_num = mouse_in_slot(mouse_window, mouse_inven);
	if (slot_num == -1)
	{
		/*	ISSUE WITH IMMPLEMENTATION, WILL DROP ITEM WHEN CLICKING ON WINDOW BUT NOT A SLOT
		// Drop items in hand
		for (int i = 0; i < hand.item.current_capacity; i++)
		{
			GameObject* go = new GameObject();
			go->unserialize_json(&ResourceManager::Instance()->get_prefab_data(hand.item.item->get_prefab_file_name()));
			SceneManager::Instance()->instantiate_gameobject(go);
		}
		*/
		return;
	}
	if (hand.item.item)
	{
		if (!mouse_inven->check_compatability(slot_num, hand.item.item))
			return;
		if (mouse_inven->get_item(slot_num))
		{
			// check if same item
			Inventory::InventoryItem temp = hand.item;
			if (mouse_inven->get_item(slot_num) == hand.item.item)
			{
				int left_over = mouse_inven->add_item(slot_num, temp.item, temp.current_capacity);
				if (left_over == 0)
					hand.clear();
				else
				{
					hand.item.current_capacity = left_over;
				}
			}
			else
			{
				hand.item.current_capacity = mouse_inven->get_capacity(slot_num);
				hand.item.item = mouse_inven->get_item(slot_num);
				hand.index = slot_num;
				hand.inventory = mouse_inven;
				hand.window = mouse_window;

				mouse_inven->remove_item(slot_num, hand.item.current_capacity);
				mouse_inven->add_item(slot_num, temp.item, temp.current_capacity);
			}
		}
		else
		{
			// place into empty slot
			mouse_inven->add_item(slot_num, hand.item.item, hand.item.current_capacity);
			hand.clear();
		}
	}
	else
	{
		if (mouse_inven->get_item(slot_num))
		{
			hand.item.item = mouse_inven->get_item(slot_num);
			hand.item.current_capacity = mouse_inven->get_capacity(slot_num);
			hand.index = slot_num;
			hand.inventory = mouse_inven;
			hand.window = mouse_window;

			hand.image.set_position(Vector2f
			(
									core::Input::Instance()->mouse_position().x - hand.image_size.x,
									core::Input::Instance()->mouse_position().y - hand.image_size.y)
			);
			hand.image.setTexture(hand.item.item->get_texture());
			hand.image.setTextureRect(hand.item.item->get_rect());
			hand.image.setFillColor(sf::Color::White);
			hand.render = true;

			mouse_inven->remove_item(slot_num, hand.item.current_capacity);
		}
	}
}

void InventoryGUIController::handle_right_click()
{
	InventoryWindow* mouse_window = nullptr;
	Inventory* mouse_inven = nullptr;

	int slot_num = mouse_in_slot(mouse_window, mouse_inven);
	if (slot_num == -1)
	{
		return;
	}

	if (hand.item.item)
	{
		if (mouse_inven->check_compatability(slot_num, hand.item.item))
			// place one item if possible
		{
			if (mouse_inven->get_item(slot_num))
			{
				if (mouse_inven->get_item(slot_num) != hand.item.item)
					return;

				int left_over = mouse_inven->add_item(slot_num, hand.item.item, 1);
				if (left_over == 1)
					return;

				hand.item.current_capacity--;

				if (hand.item.current_capacity == 0)
					hand.clear();

			}
			else
			{
				// place into empty slot
				mouse_inven->add_item(slot_num, hand.item.item, 1);
				hand.item.current_capacity--;
				if (hand.item.current_capacity == 0)
					hand.clear();
			}
		}
	}
	else
	{
		// take half stack
		if (mouse_inven->get_item(slot_num))
		{
			int half = 0;
			if (mouse_inven->get_capacity(slot_num) == 1)
				half = 1;
			else
				half = mouse_inven->get_capacity(slot_num) * .5f;

			hand.item.item = mouse_inven->get_item(slot_num);
			hand.item.current_capacity = half;
			hand.index = slot_num;
			hand.inventory = mouse_inven;
			hand.window = mouse_window;

			hand.image.set_position(Vector2f
			(
									core::Input::Instance()->mouse_position().x - hand.image_size.x,
									core::Input::Instance()->mouse_position().y - hand.image_size.y)
			);
			hand.image.setTexture(hand.item.item->get_texture());
			hand.image.setTextureRect(hand.item.item->get_rect());
			hand.image.setFillColor(sf::Color::White);
			hand.render = true;

			mouse_inven->remove_item(slot_num, half);
		}
	}

}

void InventoryGUIController::handle_shift_left_click()
{
	bool in_bounds = false;
	InventoryWindow* mouse_window = nullptr;
	Inventory* mouse_inven = nullptr;

	int slot_num = mouse_in_slot(mouse_window, mouse_inven);
	if (slot_num == -1)
		return;

	if (mouse_inven->get_item(slot_num))
	{
		hand.item.item = mouse_inven->get_item(slot_num);
		hand.item.current_capacity = mouse_inven->get_capacity(slot_num);
		hand.index = slot_num;
		hand.inventory = mouse_inven;
		hand.window = mouse_window;

		mouse_inven->remove_item(slot_num, mouse_inven->get_capacity(slot_num));
	}

	if (!hand.item.item)
		return;

	Inventory* target_inv;

	if (mouse_inven == general_inventory)
	{
		// general -> external if exists -> combat
		target_inv = combat_inventory;
		if (external_inventory)
			target_inv = external_inventory;
	}
	else if (mouse_inven == combat_inventory)
	{
		// combat -> external if exists -> general if not
		target_inv = general_inventory;
		if (external_inventory)
			target_inv = external_inventory;
	}
	else
	{
		// external -> general
		target_inv = general_inventory;
	}
	int slot = target_inv->get_first_available_include_match(hand.item.item);
	while (slot != -1)
	{
		int left_over = target_inv->add_item(slot, hand.item.item, hand.item.current_capacity);

		if (left_over == 0)
		{
			hand.clear();
			return;
		}

		hand.item.current_capacity = left_over;
		slot = target_inv->get_first_available_include_match(hand.item.item);

	}
	if (hand.item.current_capacity == 0)
		hand.clear();
	else
		hand.send_back();
}

int InventoryGUIController::mouse_in_slot(InventoryWindow*& win, Inventory*& inv)
{
	if (mouse_in_bounds(win, inv))
	{
		int slot_num = mouse_in_slot(win);
		if (slot_num != -1)
			return slot_num;
	}

	return -1;
}

bool InventoryGUIController::mouse_in_bounds(InventoryWindow*& win, Inventory*& inv)
{
	bool in_bounds = false;
	if (general_window->get_window()->mouse_in_bounds())
	{
		inv = general_inventory;
		win = general_window;
		in_bounds = true;
	}
	else if (combat_window->get_window()->mouse_in_bounds())
	{
		inv = combat_inventory;
		win = combat_window;
		in_bounds = true;
	}
	else if (external_window)
		if (external_window->get_window()->mouse_in_bounds())
		{
			inv = external_inventory;
			win = external_window;
			in_bounds = true;
		}
	return in_bounds;

}

int InventoryGUIController::mouse_in_slot(InventoryWindow*& win)
{
	std::vector<GUI::Panel*> slots = win->get_slots();
	for (int i = 0; i < slots.size(); i++)
		if (slots[i]->mouse_in_bounds())
			return i;


	return -1;
}


}
