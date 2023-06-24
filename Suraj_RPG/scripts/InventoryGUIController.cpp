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
	general_inventory = game_object->get_child("GeneralInventory").lock()->get_component<Inventory>();
	general_window = game_object->get_child("GeneralInventory").lock()->get_component<InventoryWindow>();

	combat_inventory = game_object->get_child("CombatInventory").lock()->get_component<Inventory>();
	combat_window = game_object->get_child("CombatInventory").lock()->get_component<InventoryWindow>();

	external_inventory = std::shared_ptr<Inventory>(nullptr);
	external_window = std::shared_ptr<InventoryWindow>(nullptr);

}

void InventoryGUIController::awake()
{
	general_window.lock()->set_position(Vector2f(300, 300));
	combat_window.lock()->set_position(Vector2f(100, 300));

	general_window.lock()->set_render(false);
	combat_window.lock()->set_render(false);


	GUI::Panel* p = general_window.lock()->get_slots()[0];
	hand.init(p->get_position(), Vector2f(p->get_width(), p->get_height()));

	Renderer::Instance()->add_ui(Renderer::RenderObject(&hand.image, hand.render, hand.layer, hand.z_order));

}

void InventoryGUIController::update()
{
	if (!general_window.lock()->get_render())
		return;

	if (hand.item.item.lock())
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
			general_window.lock()->toggle_render();
			break;
		case bm98::InventoryNS::WindowToggle::COMBAT_INVENTORY:
			combat_window.lock()->toggle_render();
			break;
		case bm98::InventoryNS::WindowToggle::FULL_INVENTORY:
			if (general_window.lock()->get_render() ^ combat_window.lock()->get_render())
			{
				general_window.lock()->set_render(true);
				combat_window.lock()->set_render(true);
			}
			else if (general_window.lock()->get_render() || combat_window.lock()->get_render())
			{
				general_window.lock()->set_render(false);
				combat_window.lock()->set_render(false);

				hand.send_back();
			}
			else
			{
				general_window.lock()->set_render(true);
				combat_window.lock()->set_render(true);
			}
			break;
		case bm98::InventoryNS::WindowToggle::EXTERNAL_INVENTORY:
			if (external_window.lock())
				external_window.lock()->toggle_render();
			break;
		case bm98::InventoryNS::WindowToggle::CLOSE_ALL:
			general_window.lock()->set_render(false);
			combat_window.lock()->set_render(false);
			if (external_window.lock())
				external_window.lock()->set_render(false);

			hand.send_back();
			break;
		case bm98::InventoryNS::WindowToggle::OPEN_ALL:
			general_window.lock()->set_render(true);
			combat_window.lock()->set_render(true);
			if (external_window.lock())
				external_window.lock()->set_render(true);
		default:
			break;
	}
}

void InventoryGUIController::set_external(std::shared_ptr<InventoryWindow> inv)
{
	external_window = inv;
	if (!external_window.lock())
	{
		external_inventory = std::shared_ptr<Inventory>(nullptr);
	}
	else
	{
		external_inventory = inv->get_inventory();
		external_window.lock()->set_position(Vector2f(400, 50));
		external_window.lock()->refresh_window();
	}

}

void InventoryGUIController::remove_external()
{
	if (external_window.lock())
	{
		external_window.lock()->set_position(Vector2f(0, 0));
		external_window.lock()->set_render(false);
		external_window.lock()->refresh_window();
	}
	external_inventory = std::shared_ptr<Inventory>(nullptr);
	external_window = std::shared_ptr<InventoryWindow>(nullptr);
}

void InventoryGUIController::handle_left_click()
{
	std::shared_ptr<InventoryWindow> mouse_window = std::shared_ptr<InventoryWindow>(nullptr);
	std::shared_ptr<Inventory> mouse_inven = std::shared_ptr<Inventory>(nullptr);

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
	if (hand.item.item.lock())
	{
		if (!mouse_inven->check_compatability(slot_num, hand.item.item.lock()))
			return;
		if (mouse_inven->get_item(slot_num).lock())
		{
			// check if same item
			Inventory::InventoryItem temp = hand.item;
			if (mouse_inven->get_item(slot_num).lock() == hand.item.item.lock())
			{
				int left_over = mouse_inven->add_item(slot_num, temp.item.lock(), temp.current_capacity);
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
				mouse_inven->add_item(slot_num, temp.item.lock(), temp.current_capacity);
			}
		}
		else
		{
			// place into empty slot
			mouse_inven->add_item(slot_num, hand.item.item.lock(), hand.item.current_capacity);
			hand.clear();
		}
	}
	else
	{
		if (mouse_inven->get_item(slot_num).lock())
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
			hand.image.setTexture(hand.item.item.lock()->get_texture());
			hand.image.setTextureRect(hand.item.item.lock()->get_rect());
			hand.image.setFillColor(sf::Color::White);
			hand.render = true;

			mouse_inven->remove_item(slot_num, hand.item.current_capacity);
		}
	}
}

void InventoryGUIController::handle_right_click()
{
	std::shared_ptr<InventoryWindow> mouse_window = std::shared_ptr<InventoryWindow>(nullptr);
	std::shared_ptr<Inventory> mouse_inven = std::shared_ptr<Inventory>(nullptr);

	int slot_num = mouse_in_slot(mouse_window, mouse_inven);
	if (slot_num == -1)
	{
		return;
	}

	if (hand.item.item.lock())
	{
		if (mouse_inven->check_compatability(slot_num, hand.item.item.lock()))
			// place one item if possible
		{
			if (mouse_inven->get_item(slot_num).lock())
			{
				if (mouse_inven->get_item(slot_num).lock() != hand.item.item.lock())
					return;

				int left_over = mouse_inven->add_item(slot_num, hand.item.item.lock(), 1);
				if (left_over == 1)
					return;

				hand.item.current_capacity--;

				if (hand.item.current_capacity == 0)
					hand.clear();

			}
			else
			{
				// place into empty slot
				mouse_inven->add_item(slot_num, hand.item.item.lock(), 1);
				hand.item.current_capacity--;
				if (hand.item.current_capacity == 0)
					hand.clear();
			}
		}
	}
	else
	{
		// take half stack
		if (mouse_inven->get_item(slot_num).lock())
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
			hand.image.setTexture(hand.item.item.lock()->get_texture());
			hand.image.setTextureRect(hand.item.item.lock()->get_rect());
			hand.image.setFillColor(sf::Color::White);
			hand.render = true;

			mouse_inven->remove_item(slot_num, half);
		}
	}

}

void InventoryGUIController::handle_shift_left_click()
{
	bool in_bounds = false;
	std::shared_ptr<InventoryWindow> mouse_window = std::shared_ptr<InventoryWindow>(nullptr);
	std::shared_ptr<Inventory> mouse_inven = std::shared_ptr<Inventory>(nullptr);

	int slot_num = mouse_in_slot(mouse_window, mouse_inven);
	if (slot_num == -1)
		return;

	if (mouse_inven->get_item(slot_num).lock())
	{
		hand.item.item = mouse_inven->get_item(slot_num);
		hand.item.current_capacity = mouse_inven->get_capacity(slot_num);
		hand.index = slot_num;
		hand.inventory = mouse_inven;
		hand.window = mouse_window;

		mouse_inven->remove_item(slot_num, mouse_inven->get_capacity(slot_num));
	}

	if (!hand.item.item.lock())
		return;

	std::weak_ptr<Inventory> target_inv;

	if (mouse_inven == general_inventory.lock())
	{
		// general -> external if exists -> combat
		target_inv = combat_inventory.lock();
		if (external_inventory.lock())
			target_inv = external_inventory;
	}
	else if (mouse_inven == combat_inventory.lock())
	{
		// combat -> external if exists -> general if not
		target_inv = general_inventory;
		if (external_inventory.lock())
			target_inv = external_inventory;
	}
	else
	{
		// external -> general
		target_inv = general_inventory;
	}
	int slot = target_inv.lock()->get_first_available_include_match(hand.item.item.lock());
	while (slot != -1)
	{
		int left_over = target_inv.lock()->add_item(slot, hand.item.item.lock(), hand.item.current_capacity);

		if (left_over == 0)
		{
			hand.clear();
			return;
		}

		hand.item.current_capacity = left_over;
		slot = target_inv.lock()->get_first_available_include_match(hand.item.item.lock());

	}
	if (hand.item.current_capacity == 0)
		hand.clear();
	else
		hand.send_back();
}

int InventoryGUIController::mouse_in_slot(std::shared_ptr<InventoryWindow>& win, std::shared_ptr<Inventory>& inv)
{
	if (mouse_in_bounds(win, inv))
	{
		int slot_num = mouse_in_slot(win);
		if (slot_num != -1)
			return slot_num;
	}

	return -1;
}

bool InventoryGUIController::mouse_in_bounds(std::shared_ptr<InventoryWindow>& win, std::shared_ptr<Inventory>& inv)
{
	bool in_bounds = false;
	if (general_window.lock()->get_window()->mouse_in_bounds())
	{
		inv = general_inventory.lock();
		win = general_window.lock();
		in_bounds = true;
	}
	else if (combat_window.lock()->get_window()->mouse_in_bounds())
	{
		inv = combat_inventory.lock();
		win = combat_window.lock();
		in_bounds = true;
	}
	else if (external_window.lock())
		if (external_window.lock()->get_window()->mouse_in_bounds())
		{
			inv = external_inventory.lock();
			win = external_window.lock();
			in_bounds = true;
		}
	return in_bounds;

}

int InventoryGUIController::mouse_in_slot(std::shared_ptr<InventoryWindow>& win)
{
	std::vector<GUI::Panel*> slots = win->get_slots();
	for (int i = 0; i < slots.size(); i++)
		if (slots[i]->mouse_in_bounds())
			return i;


	return -1;
}


}
