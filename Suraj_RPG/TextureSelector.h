#pragma once
#include "Gui.h"
namespace bm98::GUI
{
// WORTHWILE NOTING, THIS DOES CLASS DOES NOT YET ACCOUNT FOR
// FOR SENDING VIEW TO ITS OBJECTS
class TextureSelector :
    public GUIObject
{
public:
	TextureSelector(float x, float y, float width, float height,
		float grid_size, const sf::Texture* texture_sheet,
		std::vector<std::string> tile_sets);
	TextureSelector();
	virtual ~TextureSelector();

	void init(float x, float y, float width, float height,
		float grid_size, const sf::Texture* texture_sheet, 
		std::vector<std::string> tile_sets);

	const bool& mouse_in_bounds();
	const bool& mouse_in_container();
	/// <summary>
	/// Returns int rect containing texture in sprite sheet which is highlighted.
	/// </summary>
	const sf::IntRect& get_texture_rect() const;
	sf::IntRect convert_rect_to_grid(sf::IntRect rect);
	const Sorting::Layer get_layer() const;
	DropDownList* get_layer_dropdown();
	DropDownList* get_tiletype_selector();
	DropDownList* get_tileset_selector();
	Checkbox* get_collision_checkbox();
	Checkbox* get_animation_checkbox();

	void toggle_hidden();

	void init_view();
	void init_dropdowns(std::vector<std::string> list);
	void init_checkboxes(float size);
	void set_texture_sheet(sf::Texture* texture);

	void set_texture_rect();

	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	/// <summary> Clear the texture selector. </summary>
	void clear();
	
	

protected:

private:
	sf::View* texture_view;
	bool texture_view_render = false;

	sf::Sprite sheet;
	sf::Font* font;

	GUI::Panel* texture_selector_panel;
	GUI::Panel* sheet_panel;

	sf::RectangleShape selector;
	sf::IntRect texture_rect;
	sf::Vector2u mouse_pos_grid;

	float grid_size;
};

}