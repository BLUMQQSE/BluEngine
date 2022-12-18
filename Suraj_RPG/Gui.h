#pragma once
#include "IRenderable.h"
#include "globals.h"
enum class ButtonState
{
	BTN_IDLE = 0,
	BTN_HOVERED,
	BTN_PRESSED
};
namespace bm98
{
class IRenderable;
}
namespace bm98::GUI
{
class GUIObject : public IRenderable
{
public:
	GUIObject()
	{

	}
	virtual ~GUIObject()
	{
	}
	virtual void update()
	{

	}
	virtual void add_to_buffer(sf::View* = nullptr)
	{

	}
	virtual void set_position(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	virtual sf::Vector2f get_position()
	{
		return position;
	}
	virtual void update_sfml(sf::Event sfEvent)
	{

	}

	virtual void set_view(sf::View* view)
	{
		set_view(view);
	}
protected:
	sf::Vector2f position;
	//sf::View* view;
};

class Button : public GUIObject
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

	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;
	virtual void set_position(float x, float y) override;

	const bool is_pressed() const;
	bool mouse_in_bounds();
	const std::string get_text() const;
	const short unsigned& get_id() const;
	const float get_height() const;
	const sf::Vector2f get_position() const;

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

class DropDownList : public GUIObject
{

public:
	DropDownList(float x, float y, float width, float height, sf::Font& font, 
		std::vector<std::string> list, unsigned default_index = 0, int char_size = 14);
	~DropDownList();

	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;
	virtual void set_position(float x, float y) override;
	virtual void set_view(sf::View* view = nullptr) override;

	void toggle_list(bool toggle);

	bool changed_selection();

	const bool mouse_in_bounds();

	// NOTE: this could be a point of failure for inconsistency,
	// tho with how i send in string vector to constructor I don't believe it should be
	void set_selected_index(int index);

	const short unsigned& get_selected_index() const;
	const Button* get_selected_button() const;
private:
	sf::Font font;
	Button* active_selection;
	std::vector<Button*> list;
	bool show_list;
	bool selection_change;
	float width;
	float height;
		


};

class Checkbox : public GUIObject
{
public:
	Checkbox(float x, float y, float size);
	virtual ~Checkbox();

	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;
	virtual void set_position(float x, float y) override;

	bool is_checked();

private:
	sf::RectangleShape box;
	sf::CircleShape check;
	float size;

	bool checked;
};

class ScrollView : public GUIObject
{
public:
	ScrollView(float x, float y, float width, float height, bool vertical = true, bool horizontal = false);
	virtual ~ScrollView();

	sf::View& get_scroll_view();
	
	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;
	virtual void set_position(float x, float y) override;


private:
	sf::RectangleShape bounds;
	sf::RectangleShape vertical_handle;
	sf::RectangleShape horizontal_handle;
	sf::View* scroll_view;
	sf::IntRect viewport;

	float width;
	float height;
	bool vertical;
	bool horizontal;

};

// WORTHWILE NOTING, THIS DOES CLASS DOES NOT YET ACCOUNT FOR
// FOR SENDING VIEW TO ITS OBJECTS
class TextureSelector : public GUIObject
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

	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

protected:

private:
	sf::View* text_view;
	bool text_view_render = false;

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

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class InputBox : public GUIObject
{

public:
	InputBox(float x, float y, float width, float height, float character_size, 
		sf::Font& font, sf::Color color = sf::Color::Black, bool selected = false);
	virtual ~InputBox();

	virtual void update_sfml(sf::Event sfEvent) override;
	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;
	virtual void set_position(float x, float y) override;

	const bool is_selected() const;
	void set_selected(bool sel);

	std::string get_text();
	float get_text_value();

	void set_text(std::string value);

private:
	sf::RectangleShape box;
	sf::Text text;
	std::ostringstream text_string;
	bool selected;
	//int limit;
	
	float tick_timer = .5f;
	float tick_delta = 0;
	bool tick_on = false;

	void input_logic(int char_typed);
	void delete_last_char();


};

class Label : public GUIObject
{
public:
	Label(float x, float y, float character_size, 
		sf::Font& font, std::string text, sf::Color color = sf::Color::Black);
	virtual ~Label();

	virtual void add_to_buffer(sf::View* = nullptr) override;
	virtual void set_position(float x, float y) override;

private:
	sf::Text text_content;

};

class Panel : public GUIObject
{
public:
	Panel(float x, float y, float width, float height,
		sf::Color color = sf::Color(180, 180, 180, 255));
	virtual ~Panel();

	const bool is_active() const;
	void toggle_active();
	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;
	virtual void set_position(float x, float y);
	virtual void update_sfml(sf::Event sfEvent) override;
	virtual void set_view(sf::View* view = nullptr) override;

	std::unordered_map<std::string, GUIObject*> get_content();
	void add_element(std::string key, GUIObject* gui_object);
	void remove_element(std::string key);
	
	void add_scroll_view(std::string key, ScrollView* sv);


	float get_width();
	float get_height();

	GUIObject* get_element(std::string key);
	DropDownList* get_dropdown(std::string key);
	InputBox* get_inputbox(std::string key);
	Checkbox* get_checkbox(std::string key);
	Button* get_button(std::string key);
	Panel* get_panel(std::string key);

	void clear();

	//temporary
	void display_mouse_pos(sf::Font& font);
	bool mouse_in_bounds();

private:
	bool active;
	sf::RectangleShape panel_shape;
	std::unordered_map<std::string, GUIObject*> content;

};



}