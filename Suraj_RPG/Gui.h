#pragma once
#include "IRenderable.h"
#include "globals.h"
#include "Math.h"

enum class ButtonState
{
	BTN_IDLE = 0,
	BTN_HOVERED,
	BTN_PRESSED
};
namespace bm98
{
class Panel;
class IRenderable;
}
namespace bm98::GUI
{

class GUIObject : public IRenderable
{
public:
	GUIObject() {}
	virtual ~GUIObject() {}
	virtual void update() {}
	virtual void add_to_buffer(sf::View* view = nullptr) { set_view(view); }
	virtual void set_position(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	virtual sf::Vector2f get_position() { return position; }
	virtual void update_sfml(sf::Event sfEvent) {}

protected:
	sf::Vector2f position;
};


/// <summary>
/// Enhanced sf::Text replacement which allows alterability of any character within the string.
/// Should be used only when necessary as the draw calls for each letter is time consuming : Big hit on performance.
/// </summary>
class RichText : public GUI::GUIObject
	//: public sf::Drawable
{
public:
	RichText();
	RichText(Vector2f pos, std::string text, unsigned character_size,
			 int wrap_length = 100000, sf::Color fill_color = sf::Color::Black,
			 sf::Color outline_color = sf::Color::Transparent);
	virtual ~RichText();

	/// <summary>
	/// Acts as a constructor, resetting all values in the rich text.
	/// </summary>
	void reinit(Vector2f pos, std::string text, unsigned character_size,
				int wrap_length = 100000, sf::Color fill_color = sf::Color::Black,
				sf::Color outline_color = sf::Color::Transparent);

	void set_string(std::string text, int index = -1);
	std::string get_string(int index = -1)
	{
		std::string result;
		for (auto s : string)
			result.append(s.first.getString());
		return result;
	}

	virtual void set_view(sf::View* view = nullptr) override { set_view(view, -1); }
	void set_view(sf::View* view, int index);
	virtual void set_render(bool render) override { set_render(render, -1); }
	void set_render(bool render, int index);
	void set_font(const sf::Font& font, int index = -1);
	void set_character_size(unsigned size, int index = -1);
	void set_style(sf::Text::Style style, int index = -1, int end_index = -1);
	void set_fill_color(sf::Color color, int index = -1);
	void set_outline_color(sf::Color color, int index = -1);
	void set_outline_thickness(float thickness, int index = -1);
	virtual void set_position(float x, float y) override { set_position(Vector2f(x, y), -1); }
	void set_position(Vector2f pos, int index);

	virtual sf::Vector2f get_position() override { return get_position(-1); }
	Vector2f get_position(int index);
	sf::Color get_fill_color(int index = -1);
	sf::Color get_outline_color(int index = -1);
	Vector2f get_center(int index = -1);

	void move(Vector2f offset, int index = -1);
	void rotate(float angle, int index = -1);


	/*
	Default Behaviors:
		move_and_fade
		raindbow
		explode (all letters fly from where start)
		grow
	*/

private:

	std::vector<std::pair<sf::Text, IRenderable>> string;

	bool valid_index(int index) { return index > 0 && index < string.size(); }

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

	void reinit(float x, float y, float width, float height, sf::Font* font,
				std::string text, unsigned character_size, sf::Color text_idle, sf::Color text_hover,
				sf::Color text_active, sf::Color idle_color, sf::Color hover_color,
				sf::Color active_color,
				sf::Color outline_idle_color = sf::Color::Transparent,
				sf::Color outline_hover_color = sf::Color::Transparent,
				sf::Color outline_active_color = sf::Color::Transparent, short unsigned id = 0);

	virtual void update() override;
	virtual void set_position(float x, float y) override;

	const bool is_pressed();
	bool mouse_in_bounds();
	const std::string get_text() const;
	const short unsigned& get_id() const;
	const float get_height() const;
	sf::Vector2f get_position();

	void set_pressed();
	void set_text(const std::string text);
	void set_id(const short unsigned id);

private:
	ButtonState button_state;
	short unsigned id;

	sf::RectangleShape shape;
	//FloatConvex shape;
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

	virtual void set_render(bool render) override;

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
	void set_checked(bool checked);

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
	Label() {}
	Label(float x, float y, float character_size, 
		sf::Font& font, std::string text, sf::Color color = sf::Color::Black);
	virtual ~Label();

	virtual void add_to_buffer(sf::View* = nullptr) override;
	virtual void set_position(float x, float y) override;
	void set_text(std::string new_text);
	std::string get_text() { return text_content.getString(); }
	sf::Drawable* get_drawable() { return &text_content; }

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

	virtual void set_render(bool render) override;
	void set_fill_color(sf::Color color) { panel_shape.setFillColor(color); }

	void apply_texture(sf::Texture* texture, sf::IntRect rect);

	std::unordered_map<std::string, GUIObject*> get_content();
	void add_element(std::string key, GUIObject* gui_object);
	void remove_element(std::string key);
	
	void add_scroll_view(std::string key, ScrollView* sv);


	float get_width();
	float get_height();

	sf::RectangleShape& get_shape() { return panel_shape; }
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

#define SLIDER_HEIGHT 30

class Slider : public GUIObject
{
public:
	Slider();
	Slider(float x, float y, float width, float min_vlaue, float max_value,
		float default_value = 0, bool whole_numbers = false);
	virtual ~Slider();

	virtual void update() override;

	bool mouse_in_bounds();
	bool mouse_on_slider();
	const float get_value() const;
	const float get_width() const;


private:
	sf::RectangleShape shape;
	sf::RectangleShape slider_back;
	sf::RectangleShape slider;

	bool whole_numbers;
	bool held = false;
	
	float width;

	float min_val;
	float max_val;
	float current_val;
	
};

class TransformMover : public GUIObject
{
public:
	TransformMover(Vector2f position, Vector2f size);
	virtual ~TransformMover();

	virtual void update() override;
	virtual void set_position(float x, float y) override;

	inline bool is_held() { return horizontal_held || vertical_held; }
	bool mouse_in_bounds();
	virtual sf::Vector2f get_position() override;

private:
	std::array<FloatConvex, 2> horizontal_arrow;
	std::array<FloatConvex, 2> vertical_arrow;
	FloatConvex full_movement;

	Vector2f offset = Vector2f::Zero();
	bool horizontal_held;
	bool vertical_held;
	Vector2f size;

};

class ProgressBar : public GUIObject
{
public:
	ProgressBar();
	virtual ~ProgressBar();

	void set_position(Vector2f position);
	void set_size(Vector2f size);
	void set_color(sf::Color color);
	void set_percentage(float percentage);

private:
	FloatConvex progress_shape;
	FloatConvex full_state_shape;

	float percentage = 100;
	Vector2f size = Vector2f::Zero();
	Vector2f position = Vector2f::Zero();
};

}