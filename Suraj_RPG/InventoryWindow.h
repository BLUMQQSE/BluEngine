#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "Gui.h"

namespace bm98
{
class Inventory;

using namespace core;
class InventoryWindow : public Component, public Listener
{

public:
	virtual void init() override;


	inline void set_render(bool render) { panel->set_render(render); }
	inline void toggle_render() { panel->set_render(!panel->get_render()); };
	inline bool get_render() { return panel->get_render(); }

	/// <returns>The inventory window panel.</returns>
	inline GUI::Panel* get_window() { return panel; }
	inline Inventory* get_inventory() { return inventory; }

	inline std::vector<GUI::Panel*> get_slots() { return slots; }
	inline std::vector<GUI::Panel*> get_images() { return images; }
	inline std::vector<GUI::Label*> get_counts() { return counts; }

	void set_position(Vector2f pos);
	void move(Vector2f change);

	void refresh_window();

private:
	Inventory* inventory;
	GUI::Panel* panel;
	std::vector<GUI::Panel*> slots;
	std::vector<GUI::Panel*> images;
	std::vector<GUI::Label*> counts;

	int x_slots = 0;
	int y_slots = 0;

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

	void create_inventory_window();
	void clear_inventory_window();

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;

};

}