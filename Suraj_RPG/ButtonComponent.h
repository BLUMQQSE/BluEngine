#pragma once
#include "Component.h"
#include "IRenderable.h"
namespace bm98
{
enum class ButtonState
{
	BTN_IDLE = 0,
	BTN_HOVERED,
	BTN_PRESSED
};
class ButtonComponent :
    public Component, public IRenderable
{
public:

    ButtonComponent();
	ButtonComponent(float width, float height, std::string text, unsigned character_size, 
		sf::Color text_idle, sf::Color text_hover, short unsigned id = 0);
	virtual ~ButtonComponent();

	virtual void init() override;
	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	const bool is_pressed() const;
	bool mouse_in_bounds();
	const std::string get_text() const;
	const short unsigned& get_id() const;
	const float get_height() const;
	const float get_width() const;

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

	void init_font();

};

}