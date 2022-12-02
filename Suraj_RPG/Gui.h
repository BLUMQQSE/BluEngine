#pragma once
#include "globals.h"
enum class ButtonState
{
	BTN_IDLE = 0,
	BTN_HOVERED,
	BTN_PRESSED
};

namespace bm98::GUI
{
class Button
{

public:
	Button();
	Button(float x, float y, float width, float height, sf::Font* font,
		std::string text, unsigned character_size, sf::Color text_idle, sf::Color text_hover,
		sf::Color text_active, sf::Color idle_color, sf::Color hover_color,
		sf::Color active_color, 
		sf::Color outline_idle_color = sf::Color::Transparent, 
		sf::Color outline_hover_color = sf::Color::Transparent,
		sf::Color outline_active_color = sf::Color::Transparent, short unsigned id = 0);
	virtual ~Button();

	void update();
	void add_to_buffer(sf::View* view = nullptr);

	const bool is_pressed() const;
	const bool mouse_in_bounds() const;
	const std::string get_text() const;
	const short unsigned& get_id() const;

	void set_pressed();
	void set_text(const std::string text);
	void set_id(const short unsigned id);

private:
	ButtonState button_state;
	short unsigned id;

	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text;

	sf::Color text_idle;
	sf::Color text_hover;
	sf::Color text_active;

	sf::Color idle_color;
	sf::Color hover_color;
	sf::Color active_color;

	sf::Color outline_idle_color;
	sf::Color outline_hover_color;
	sf::Color outline_active_color;

};

class DropDownList
{

public:
	DropDownList(float x, float y, float width, float height,
		sf::Font& font, std::vector<std::string> list, unsigned default_index = 0);
	~DropDownList();

	void update();
	void add_to_buffer(sf::View* view = nullptr);
	bool changed_selection();

	const bool mouse_in_bounds();
	const short unsigned& get_selected_index() const;
	const Button* get_selected_button() const;
private:
	sf::Font& font;
	Button* active_selection;
	std::vector<Button*> list;
	bool show_list;
	bool selection_change;
		


};

class Checkbox
{
public:
	Checkbox(float x, float y, float size);
	virtual ~Checkbox();

	void update();
	void add_to_buffer(sf::View* view = nullptr);

	bool is_checked();

private:
	sf::RectangleShape box;
	sf::CircleShape check;

	bool checked;
};

class ScrollView
{
public:
	ScrollView(float x, float y, float width, float height, bool vertical = true, bool horizontal = false);
	virtual ~ScrollView();

	sf::View& get_scroll_view();
	void update();
	void add_to_buffer(sf::View* view = nullptr);


private:
	sf::RectangleShape bounds;
	sf::RectangleShape vertical_handle;
	sf::RectangleShape horizontal_handle;
	sf::View scroll_view;
	sf::IntRect viewport;

	bool vertical;
	bool horizontal;

};

class TextureSelector
{
public:
	TextureSelector(float x, float y, float width, float height,
		float grid_size, const sf::Texture* texture_sheet, sf::Font& font,
		std::vector<std::string> tile_sets);
	virtual ~TextureSelector();

	const bool& mouse_in_bounds() const;
	const bool& mouse_in_container() const;
	/// <summary>
	/// Returns int rect containing texture in sprite sheet which is highlighted.
	/// </summary>
	const sf::IntRect& get_texture_rect() const;
	sf::IntRect convert_rect_to_grid(sf::IntRect rect);
	const SortingLayer get_layer() const;
	DropDownList* get_layer_dropdown();
	DropDownList* get_tiletype_selector();
	DropDownList* get_tileset_selector();
	Checkbox* get_collision_checkbox();
	Checkbox* get_animation_checkbox();

	void toggle_hidden();
	void init_buttons(float x, float y);
	void init_dropdowns(float x, float y, std::vector<std::string> list);
	void init_checkboxes(float x, float y, float size);
	void set_texture_sheet(sf::Texture* texture);

	void set_texture_rect();

	void update();
	void add_to_buffer(sf::View* view = nullptr);

protected:

private:
	sf::View view;

	sf::RectangleShape container;
	sf::RectangleShape bounds;
	sf::Sprite sheet;
	sf::Font& font;

	DropDownList* layer_selector;
	DropDownList* tile_collection_selector;
	DropDownList* tile_type_selector;
	Checkbox* collision_checkbox;
	Checkbox* animation_checkbox;

	sf::RectangleShape selector;
	sf::IntRect texture_rect;
	sf::Vector2u texture_start;
	sf::Vector2u mouse_pos_grid;

	float grid_size;
	bool hidden;

};

class InputBox
{

public:
	InputBox();
	virtual ~InputBox();

	std::string get_text();

private:
	sf::Text text;


};

}