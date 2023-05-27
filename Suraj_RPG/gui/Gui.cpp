#include "../pch.h"
#include "Gui.h"

#include "../core/Time.h"
#include "../core/Input.h"
#include "../core/Renderer.h"
#include "../core/Debug.h"
#include "../core/ResourceManager.h"

namespace bm98::GUI
{
using namespace core;

#pragma region RICHTEXT

RichText::RichText()
{
	string.resize(1);
	set_render(false);
}

RichText::RichText(Vector2f pos, std::string text, unsigned character_size, int wrap_length, sf::Color fill_color,
				   sf::Color outline_color)
{
	reinit(pos, text, character_size, wrap_length, fill_color, outline_color);
}

RichText::~RichText()
{
	for (auto& s : string)
	{
		Renderer::Instance()->remove_ui(&s.first);
	}
}

void RichText::reinit(Vector2f pos, std::string text, unsigned character_size,
					  int wrap_length, sf::Color fill_color,
					  sf::Color outline_color)
{
	set_render(true);

	for (auto& s : string)
	{
		Renderer::Instance()->remove_ui(&s.first);
	}

	string.clear();
	string.resize(text.size());

	float width = 0;
	float height = 0;

	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == '\n')
		{
			if (i > 0)
			{
				height += string[i - 1].first.getCharacterSize();
				width = 0;
			}
			continue;
		}
		else if (i != 0)
		{
			if (i % wrap_length == 0)
			{
				height += string[i - 1].first.getCharacterSize();
				width = 0;
			}
		}
		string[i].second = IRenderable(Sorting::Layer::UI, 2, true);

		string[i].first.setString(text[i]);
		string[i].first.setFillColor(fill_color);
		string[i].first.setOutlineColor(outline_color);
		string[i].first.setOutlineThickness(1.f);

		string[i].first.setPosition((int)(pos.x + width), (int)(pos.y +height));

		string[i].first.setFont(ResourceManager::Instance()->get_font("calibri-regular.ttf"));
		string[i].first.setCharacterSize(character_size);

		width += string[i].first.findCharacterPos(string[i].first.getString().getSize()).x - string[i].first.findCharacterPos(0).x;
		
	}

	for (auto& s : string)
	{
		Renderer::Instance()->add_ui(Renderer::RenderObject(&s.first, &s.second));
	}
}

void RichText::set_string(std::string text, int index)
{
	sf::Color fill = string[0].first.getFillColor();
	sf::Color out = string[0].first.getOutlineColor();
	float thicc = string[0].first.getOutlineThickness();
	int size = string[0].first.getCharacterSize();

	Vector2f pos = string[0].first.getPosition();

	string.resize(text.length());

	float width = 0;

	for (int i = 0; i < text.length(); i++)
	{
		string[i].first.setString(text[i]);
		string[i].first.setFillColor(fill);
		string[i].first.setOutlineColor(out);
		string[i].first.setOutlineThickness(thicc);

		string[i].first.setPosition((int)(pos.x + width), (int)pos.y);

		string[i].first.setFont(ResourceManager::Instance()->get_font("calibri-regular.ttf"));
		string[i].first.setCharacterSize(size);

		width += string[i].first.findCharacterPos(string[i].first.getString().getSize()).x - string[i].first.findCharacterPos(0).x;

	}
}

void RichText::set_view(sf::View* view, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.second.set_view(view);
		return;
	}
	if (!valid_index(index))
		return;
	string[index].second.set_view(view);

}

void RichText::set_render(bool render, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
		{
			s.second.set_render(render);
		}
		return;
	}
	if (!valid_index(index))
		return;
	string[index].second.set_render(render);
}

void RichText::set_font(const sf::Font& font, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.first.setFont(font);
		return;
	}

	if (!valid_index(index))
		return;

	string[index].first.setFont(font);

}

void RichText::set_character_size(unsigned size, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.first.setCharacterSize(size);
		return;
	}

	if (!valid_index(index))
		return;

	string[index].first.setCharacterSize(size);
}

void RichText::set_style(sf::Text::Style style, int index, int end_index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.first.setStyle(style);
		return;
	}

	if (!valid_index(index))
		return;

	if (end_index != -1)
	{
		if (end_index > 0)
		{
			end_index = std::min(end_index, (int)(string.size() - 1));

			for (int i = index; i <= end_index; i++)
				string[i].first.setStyle(style);
			return;
		}
	}

	string[index].first.setStyle(style);
}

void RichText::set_fill_color(sf::Color color, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.first.setFillColor(color);
		return;
	}
	
	if (!valid_index(index))
		return;

	string[index].first.setFillColor(color);
}

void RichText::set_outline_color(sf::Color color, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.first.setOutlineColor(color);
		return;
	}

	if (!valid_index(index))
		return;

	string[index].first.setOutlineColor(color);
}

void RichText::set_outline_thickness(float thickness, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.first.setOutlineThickness(thickness);
		return;
	}

	if (!valid_index(index))
		return;

	string[index].first.setOutlineThickness(thickness);
}

void RichText::set_position(Vector2f pos, int index)
{
	if (index == -1)
	{
		std::string temp_string = get_string();
		int width = 0;
		for (int i = 0; i < temp_string.length(); i++)
		{
			string[i].first.setPosition((int)(pos.x + width), (int)pos.y);
			width += string[i].first.findCharacterPos(string[i].first.getString().getSize()).x - string[i].first.findCharacterPos(0).x;
		}
		return;
	}

	if (!valid_index(index))
		return;

	string[index].first.setPosition(pos);
}

Vector2f RichText::get_position(int index)
{

	if (index == -1)
		return string[0].first.getPosition();
	
	if (!valid_index(index))
		return Vector2f(INFINITY, INFINITY);
	else
		return string[index].first.getPosition();

}

sf::Color RichText::get_fill_color(int index)
{
	if (index == -1)
		return string[0].first.getFillColor();

	if (!valid_index(index))
		return sf::Color::White;

	else
		return string[index].first.getFillColor();
}

sf::Color RichText::get_outline_color(int index)
{
	if (index == -1)
		return string[0].first.getOutlineColor();

	if (!valid_index(index))
		return sf::Color::White;

	else
		return string[index].first.getOutlineColor();
}

Vector2f RichText::get_center(int index)
{
	if (index == -1)
	{
		Vector2f result;

		for (int i = 0; i < string.size(); i++)
		{
			result += Vector2f
			(
				string[i].first.getPosition().x + string[i].first.findCharacterPos(string[i].first.getString().getSize()).x - string[i].first.findCharacterPos(0).x / 2,
				string[i].first.getPosition().y + string[i].first.getCharacterSize() / 2
			);
		}
		return result;
	}
	
	return Vector2f
	(
		string[index].first.getPosition().x + string[index].first.findCharacterPos(string[index].first.getString().getSize()).x - string[index].first.findCharacterPos(0).x / 2,
		string[index].first.getPosition().y + string[index].first.getCharacterSize() / 2
	);

}

void RichText::move(Vector2f offset, int index)
{
	if (index == -1)
	{
		std::string temp_string = get_string();
		for (int i = 0; i < temp_string.length(); i++)
		{
			string[i].first.setPosition(string[i].first.getPosition().x + offset.x, string[i].first.getPosition().y + offset.y);
		}
		return;
	}

	if (!valid_index(index))
		return;

	string[index].first.setPosition(string[index].first.getPosition() + offset);
}

void RichText::rotate(float angle, int index)
{
	if (index == -1)
	{
		for (auto& s : string)
			s.first.rotate(angle);
		return;
	}

	if (!valid_index(index))
		return;

	string[index].first.rotate(angle);
}


/*
void RichText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& s : string)
	{
		target.draw(s, states);
	}
}
*/

#pragma endregion

#pragma region BUTTON

Button::Button()
{

}

Button::Button(float x, float y, float width, float height, sf::Font* font,
	std::string text, unsigned character_size, sf::Color text_idle, sf::Color text_hover,
	sf::Color text_active,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color,
	short unsigned id)
{
	reinit(x, y, width, height, font, text, character_size, text_idle, text_hover, text_active,
		   idle_color, hover_color, active_color, outline_idle_color, outline_hover_color, 
		   outline_active_color, id);
}

Button::~Button()
{
	Renderer::Instance()->remove_ui(&shape);
	Renderer::Instance()->remove_ui(&text);
}

void Button::reinit(float x, float y, float width, float height, sf::Font* font, 
					std::string text, unsigned character_size, sf::Color text_idle, sf::Color text_hover,
					sf::Color text_active, 
					sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
					sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color, 
					short unsigned id)
{
	Renderer::Instance()->remove_ui(&shape);
	Renderer::Instance()->remove_ui(&this->text);

	this->position = sf::Vector2f(x, y);
	this->id = id;
	this->button_state = ButtonState::BTN_IDLE;
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_idle_color);
	this->font = font;

	this->text.setString(text);
	this->text.setFont(*font);
	this->text.setFillColor(text_idle);
	this->text.setCharacterSize(character_size);

	this->text.setPosition(
		(int)(shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f),
		(int)(shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height) /*/ 2.f*/
	);

	this->text_idle = text_idle;
	this->text_hover = text_hover;
	this->text_active = text_active;

	this->idle_color = idle_color;
	this->hover_color = hover_color;
	this->active_color = active_color;

	this->outline_idle_color = outline_idle_color;
	this->outline_hover_color = outline_hover_color;
	this->outline_active_color = outline_active_color;

	shape.setFillColor(idle_color);

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(id + 10, false);

	Renderer::RenderObject shape_renderable = Renderer::RenderObject(&shape, get_render(), get_sorting_layer(),
																	 get_z_order(), get_view_pointer());
	shape_renderable.has_global_bounds = true;

	Renderer::Instance()->add_ui(shape_renderable);

	Renderer::Instance()->add_ui(Renderer::RenderObject(&this->text, get_render(), get_sorting_layer(),
								 get_z_order(), get_view_pointer()));
}

void Button::update()
{
	
	//std::cout << std::boolalpha << Renderer::Instance()->is_top_ui(&shape) << "\n";

	if (shape.getGlobalBounds().contains(Input::Instance()->mouse_position(get_view())) &&
		Input::Instance()->get_mouse_down() && Renderer::Instance()->is_top_ui(&shape))
	{
		button_state = ButtonState::BTN_PRESSED;
	}
	else if (shape.getGlobalBounds().contains(Input::Instance()->mouse_position(get_view())) && Renderer::Instance()->is_top_ui(&shape))
	{
		button_state = ButtonState::BTN_HOVERED;
	}
	else
	{
		button_state = ButtonState::BTN_IDLE;
	}

	switch (button_state)
	{
	case ButtonState::BTN_IDLE:
		shape.setFillColor(idle_color);
		shape.setOutlineColor(outline_idle_color);
		text.setFillColor(text_idle);
		break;
	case ButtonState::BTN_HOVERED:
		shape.setFillColor(hover_color);
		shape.setOutlineColor(outline_hover_color);
		text.setFillColor(text_hover);
		break;
	case ButtonState::BTN_PRESSED:
		shape.setFillColor(active_color);
		shape.setOutlineColor(outline_active_color);
		text.setFillColor(text_active);
		break;
	default:
		shape.setFillColor(sf::Color::Red);

	}

}

void Button::set_position(float x, float y)
{
	GUIObject::set_position(x, y);
	this->shape.setPosition(sf::Vector2f(x, y));

	this->text.setPosition(
		(int)(shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f),
		(int)(shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height) /*/ 2.f*/
	);
	
}

const bool Button::is_pressed() 
{
	return button_state == ButtonState::BTN_PRESSED;
}

bool Button::mouse_in_bounds() 
{
	return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(
		Input::Instance()->mouse_position( get_view())));
}

const std::string Button::get_text() const
{
	return text.getString();
}

const short unsigned& Button::get_id() const
{
	return id;
}

const float Button::get_height() const
{
	return shape.getGlobalBounds().height;
}

sf::Vector2f Button::get_position()
{
	return shape.getPosition();
}

void Button::set_colors(sf::Color text_idle, sf::Color text_hover,
						sf::Color text_active, sf::Color idle_color, sf::Color hover_color,
						sf::Color active_color, sf::Color outline_idle_color,
						sf::Color outline_hover_color, sf::Color outline_active_color)
{
	this->text_idle = text_idle;
	this->text_hover = text_hover;
	this->text_active = text_active;

	this->idle_color = idle_color;
	this->hover_color = hover_color;
	this->active_color = active_color;

	this->outline_idle_color = outline_idle_color;
	this->outline_hover_color = outline_hover_color;
	this->outline_active_color = outline_active_color;
}

void Button::set_pressed()
{
	button_state = ButtonState::BTN_PRESSED;
}

void Button::set_text(const std::string text)
{
	this->text.setString(text);
	this->text.setPosition(
		(int)(shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f),
		(int)(shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height) /*/ 2.f*/
	);
}

void Button::set_id(const short unsigned id)
{
	this->id = id;
}

#pragma endregion

#pragma region DROP_DOWN_LIST

DropDownList::DropDownList(float x, float y, float width, float height, sf::Font& font,
	std::vector<std::string> list, unsigned default_index, int char_size)
	: font(font), show_list(false), selection_change(false)
{
	this->position = sf::Vector2f(x, y);
	this->width = width;
	this->height = height;

	active_selection = new GUI::Button(
		x, y, width, height,
		&font, list[default_index], char_size,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50)
	);
	for (size_t i = 0; i < list.size(); i++)
	{
		this->list.push_back(
			new GUI::Button(
				x, y + ((i + 1) * height), width, height,
				&font, list[i], char_size,
				sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(250, 250, 250, 250),
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0), i
			)
		);
		this->list[i]->set_render(false);
	}

	active_selection->set_id(default_index);
}

DropDownList::~DropDownList()
{
	delete active_selection;
	for (size_t i = 0; i < list.size(); i++)
	{
		delete list[i];
	}
}

void DropDownList::update()
{
	active_selection->update();

	if (active_selection->is_pressed())
	{
		toggle_list(!show_list);
	}

	if (show_list)
	{
		for (auto& i : list)
		{
			i->update();
			if (i->is_pressed())
			{
				toggle_list(false);
				selection_change = true;
				active_selection->set_text(i->get_text());
				active_selection->set_id(i->get_id());
				return;
			}
		}
		if (Input::Instance()->get_mouse_down() && !active_selection->is_pressed())
			toggle_list(false);
		
	}
}


void DropDownList::add_to_buffer(sf::View* view)
{
	active_selection->add_to_buffer(view);

	active_selection->set_view(view);
	if(show_list)
		for (auto& b : list)
			b->set_view(view);
	/*
	if (show_list)
	{
		for (auto& i : list)
		{
			i->add_to_buffer(view);
		}
	}
	*/
}

void DropDownList::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	active_selection->set_position(x, y);
	for (size_t i = 0; i < list.size(); i++)
		list[i]->set_position(x, y + ((i + 1) * height));
		
	
}

void DropDownList::set_render(bool render)
{
	active_selection->set_render(render);
	for (int i = 0; i < list.size(); i++)
		list[i]->set_render(false);
}

void DropDownList::toggle_list(bool toggle)
{
	show_list = toggle;
	for (auto& l : list)
	{
		l->set_render(toggle);
	}
}

bool DropDownList::changed_selection()
{
	if (selection_change)
	{
		selection_change = false;
		return true;
	}
	return false;
}

const bool DropDownList::mouse_in_bounds()
{
	for (auto& f : list)
	{
		if (f->mouse_in_bounds())
			return true;
	}
	return false;
}

void DropDownList::set_selected_index(int index)
{
	active_selection->set_text(list[index]->get_text());
	active_selection->set_id(list[index]->get_id());
}

const short unsigned& DropDownList::get_selected_index() const
{
	return active_selection->get_id();
}

const Button* DropDownList::get_selected_button() const
{
	return active_selection;
}

#pragma endregion

#pragma region FLAG_DROP_DOWN_LIST

FlagDropDownList::FlagDropDownList(float x, float y, float width, float height, sf::Font& font,
								   std::vector<std::string> list, unsigned default_index, int char_size)
	: font(font), show_list(false), selection_change(false), enum_flag(list)
{
	this->position = sf::Vector2f(x, y);
	this->width = width;
	this->height = height;

	active_selection = new GUI::Button(
		x, y, width, height,
		&font, "FLAGS", char_size,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 170, 200), sf::Color(150, 150, 250, 200), sf::Color(20, 20, 120, 200),
		sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0)
	);

	active_selection->set_outline_thickness(2);

	for (size_t i = 0; i < list.size(); i++)
	{
		this->list.push_back(
			new GUI::Button(
			x, y + ((i + 1) * height), width, height,
			&font, list[i], char_size,
			sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(250, 250, 250, 250),
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
			sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0), i
		)
		);
		this->list[i]->set_render(false);
	}

	active_selection->set_id(default_index);
}

FlagDropDownList::~FlagDropDownList()
{
	delete active_selection;
	for (size_t i = 0; i < list.size(); i++)
	{
		delete list[i];
	}
}

void FlagDropDownList::update()
{
	active_selection->update();

	if (active_selection->is_pressed())
	{
		toggle_list(!show_list);
	}

	if (show_list)
	{
		for (auto& i : list)
		{
			i->update();
			if (i->is_pressed())
			{
				// check if i is already true
				int x = i->get_id();
				
				if (enum_flag.at(x))
				{
					// set false
					enum_flag[x] = false;
					i->set_colors(sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(250, 250, 250, 250),
								  sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
								  sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0));
				}
				else
				{
					// set true
					enum_flag[x]= true;
					i->set_colors(sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
								  sf::Color(70, 170, 70, 200), sf::Color(150, 250, 150, 200), sf::Color(20, 120, 20, 200),
								  sf::Color(0, 255, 0, 255), sf::Color(0, 255, 0, 255), sf::Color(0, 255, 0, 255));
				}

				// determine color
				if (enum_flag == false)
				{
					active_selection->set_colors(sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(250, 250, 250, 250),
												 sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
												 sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0));
				}
				else
				{
					active_selection->set_colors(sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
												 sf::Color(70, 170, 70, 200), sf::Color(150, 250, 150, 200), sf::Color(20, 120, 20, 200),
												 sf::Color(0, 255, 0, 255), sf::Color(0, 255, 0, 255), sf::Color(0, 255, 0, 255));
				}

				toggle_list(false);
				selection_change = true;
				//active_selection->set_text(i->get_text());
				//active_selection->set_id(i->get_id());
				return;
			}
		}
		if (Input::Instance()->get_mouse_down() && !active_selection->is_pressed())
			toggle_list(false);

	}
}

void FlagDropDownList::add_to_buffer(sf::View* view)
{
	active_selection->add_to_buffer(view);

	active_selection->set_view(view);
	if (show_list)
		for (auto& b : list)
			b->set_view(view);
	/*
	if (show_list)
	{
		for (auto& i : list)
		{
			i->add_to_buffer(view);
		}
	}
	*/
}

void FlagDropDownList::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	active_selection->set_position(x, y);
	for (size_t i = 0; i < list.size(); i++)
		list[i]->set_position(x, y + ((i + 1) * height));


}

void FlagDropDownList::set_render(bool render)
{
	active_selection->set_render(render);
	for (int i = 0; i < list.size(); i++)
		list[i]->set_render(false);
}

void FlagDropDownList::toggle_list(bool toggle)
{
	show_list = toggle;
	for (auto& l : list)
	{
		l->set_render(toggle);
	}
}

bool FlagDropDownList::changed_selection()
{
	if (selection_change)
	{
		selection_change = false;
		return true;
	}
	return false;
}

const bool FlagDropDownList::mouse_in_bounds()
{
	for (auto& f : list)
	{
		if (f->mouse_in_bounds())
			return true;
	}
	return false;
}

const std::vector<int> FlagDropDownList::get_selected_indexes()
{
	std::vector<int> result;
	
	for (int i = 0; i < enum_flag.size(); i++)
	{
		if (enum_flag.at(i))
			result.push_back(i);
	}
	return result;
}

#pragma endregion



#pragma region CHECK_BOX

Checkbox::Checkbox(float x, float y, float size)
{
	this->position = sf::Vector2f(x, y);
	this->size = size;
	float check_offset = size * .2f;
	box.setPosition(x, y);
	box.setFillColor(sf::Color(80, 80, 80, 255));
	box.setSize(sf::Vector2f(size, size));
	check.setPosition(x + check_offset, y + check_offset);
	check.setFillColor(sf::Color::Transparent);
	check.setRadius((size / 2 - check_offset));

	//check = FloatConvex::circle(Vector2f(x + check_offset, y + check_offset), size / 2 - check_offset);

	checked = false;

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(1, false);

	Renderer::RenderObject shape_renderable = Renderer::RenderObject(&box, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	shape_renderable.has_global_bounds = true;

	Renderer::Instance()->add_ui(shape_renderable);
	Renderer::Instance()->add_ui(Renderer::RenderObject(&check, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer()));
}

Checkbox::~Checkbox()
{
	Renderer::Instance()->remove_ui(&box);
	Renderer::Instance()->remove_ui(&check);
}

void Checkbox::update()
{
	if (box.getGlobalBounds().contains(Input::Instance()->mouse_position( get_view() )) &&
		Input::Instance()->get_mouse_down() && Renderer::Instance()->is_top_ui(&box))
	{
		//std::cout << "check\n";
		checked = !checked;
		if (checked)
		{
			check.setFillColor(sf::Color::White);
			return;
		}
		check.setFillColor(sf::Color::Transparent);
		return;
	}
}

void Checkbox::add_to_buffer(sf::View* view)
{
	set_view(view);
	//Renderer::add(Renderer::RenderObject(&box, render, SortingLayer::UI, 1, &view));
	//Renderer::add(Renderer::RenderObject(&check, render, SortingLayer::UI, 1, &view));
}

void Checkbox::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	float check_offset = size * .2f;
	box.setPosition(x, y);
	check.setPosition(Vector2f(x + check_offset, y + check_offset));
}

bool Checkbox::is_checked()
{
	return checked;
}

void Checkbox::set_checked(bool checked)
{
	this->checked = checked;
	if (checked)
	{
		check.setFillColor(sf::Color::White);
		return;
	}
	check.setFillColor(sf::Color::Transparent);
	return;
}

#pragma endregion

#pragma region SCROLL_VIEW

ScrollView::ScrollView(float x, float y, float width, float height, bool vertical, bool horizontal)
{
	scroll_view = new sf::View();
	this->position = sf::Vector2f(x, y);
	this->width = width;
	this->height = height;
	bounds.setPosition(x, y);
	bounds.setSize(sf::Vector2f(width, height));
	bounds.setOutlineThickness(1.f);
	bounds.setOutlineColor(sf::Color::White);
	bounds.setFillColor(sf::Color::Transparent);


	scroll_view->setSize(bounds.getSize().x, bounds.getSize().y);
	scroll_view->setCenter(bounds.getPosition().x + bounds.getSize().x / 2,
		bounds.getPosition().y + bounds.getSize().y / 2);

	float left = bounds.getPosition().x / Renderer::Instance()->get_window_size().x;
	float top = bounds.getPosition().y / Renderer::Instance()->get_window_size().y;
	float w = bounds.getSize().x / Renderer::Instance()->get_window_size().x;
	float h = bounds.getSize().y / Renderer::Instance()->get_window_size().y;

	scroll_view->setViewport(sf::FloatRect(left, top, w, h));

	this->vertical = vertical;
	this->horizontal = horizontal;
	if (vertical)
	{
		vertical_handle.setPosition(x + width - 15, y + 5);
		vertical_handle.setSize(sf::Vector2f(10, height * .2f));
	}
	if (horizontal)
	{
		horizontal_handle.setPosition(x + 5, y + height - 15);
		horizontal_handle.setSize(sf::Vector2f(width * .2f, 10));
	}
	if (vertical)
		vertical_handle.setFillColor(sf::Color::Green);
	if (horizontal)
		horizontal_handle.setFillColor(sf::Color::Green);

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(0, false);
	Renderer::Instance()->add_ui(Renderer::RenderObject(&bounds, get_render(), get_sorting_layer(),
		get_z_order(), &scroll_view));
	if (vertical)
		Renderer::Instance()->add_ui(Renderer::RenderObject(&vertical_handle, get_render(), get_sorting_layer(),
			get_z_order(), &scroll_view));
	if (horizontal)
		Renderer::Instance()->add_ui(Renderer::RenderObject(&horizontal_handle, get_render(), get_sorting_layer(),
			get_z_order(), &scroll_view));

}

ScrollView::~ScrollView()
{
	Renderer::Instance()->remove_ui(&bounds);
	if (vertical)
		Renderer::Instance()->remove_ui(&vertical_handle);
	if (horizontal)
		Renderer::Instance()->remove_ui(&horizontal_handle);
	delete scroll_view;
}

sf::View& ScrollView::get_scroll_view()
{
	return *scroll_view;
}

void ScrollView::update()
{
	/*
	if (Input::get_mouse(Input::Mouse::LEFT) && 
		vertical_handle.getGlobalBounds().contains(Input::mouse_position(&scroll_view)))
	{
		float y = vertical_handle.getPosition().y;
		vertical_handle.setPosition(vertical_handle.getPosition().x, Input::mouse_position(&scroll_view).y);
		scroll_view.move(0, y - vertical_handle.getPosition().y);
	}
	if (Input::get_mouse(Input::Mouse::LEFT) && horizontal_handle.getGlobalBounds().contains(Input::mouse_position(&scroll_view)))
	{
		float x = vertical_handle.getPosition().y;
		horizontal_handle.setPosition(Input::mouse_position(&scroll_view).x, horizontal_handle.getPosition().y);
		scroll_view.move(x - vertical_handle.getPosition().x, 0);
	}

	if (Input::get_action("SHIFT") && Input::get_mouse_scroll_delta() < 0)
		scroll_view.move(1000 * Time::delta_time(), 0);
	else if (Input::get_action("SHIFT") && Input::get_mouse_scroll_delta() > 0)
		scroll_view.move(-1000 * Time::delta_time(), 0);
	else if (Input::get_mouse_scroll_delta() < 0)
		scroll_view.move(0, 1000 * Time::delta_time());
	else if (Input::get_mouse_scroll_delta() > 0)
		scroll_view.move(0, -1000 * Time::delta_time());

		*/
}

void ScrollView::add_to_buffer(sf::View* view)
{
	/*
	Renderer::add(Renderer::RenderObject(&bounds, render, SortingLayer::UI, 0, &scroll_view));
	if (vertical)
		Renderer::add(Renderer::RenderObject(&vertical_handle, render, SortingLayer::UI, 0, &scroll_view));
	if (horizontal)
		Renderer::add(Renderer::RenderObject(&horizontal_handle, render, SortingLayer::UI, 0, &scroll_view));
*/
}

void ScrollView::set_position(float x, float y)
{
	GUIObject::set_position(x, y);
	bounds.setPosition(x, y);


	scroll_view->setCenter(bounds.getPosition().x + bounds.getSize().x / 2,
		bounds.getPosition().y + bounds.getSize().y / 2);

	float left = bounds.getPosition().x / Renderer::Instance()->get_window_size().x;
	float top = bounds.getPosition().y / Renderer::Instance()->get_window_size().y;
	float w = bounds.getSize().x / Renderer::Instance()->get_window_size().x;
	float h = bounds.getSize().y / Renderer::Instance()->get_window_size().y;

	scroll_view->setViewport(sf::FloatRect(left, top, w, h));

	this->vertical = vertical;
	this->horizontal = horizontal;
	if (vertical)
	{
		vertical_handle.setPosition(x + width - 15, y + 5);
		vertical_handle.setSize(sf::Vector2f(10, height * .2f));
	}
	if (horizontal)
	{
		horizontal_handle.setPosition(x + 5, y + height - 15);
		horizontal_handle.setSize(sf::Vector2f(width * .2f, 10));
	}
	if (vertical)
		vertical_handle.setFillColor(sf::Color::Green);
	if (horizontal)
		horizontal_handle.setFillColor(sf::Color::Green);
}

#pragma endregion

#pragma region INPUT_BOX

InputBox::InputBox(float x, float y, float width, float height, float character_size, 
	sf::Font& font, sf::Color color, bool selected)
{
	this->position = sf::Vector2f(x, y);
	//limit = static_cast<int>(std::floorf(width / 10));
	box.setSize(sf::Vector2f(width, height));
	box.setOutlineThickness(1.f);
	box.setFillColor(sf::Color::Transparent);
	box.setFillColor(sf::Color(180, 180, 180, 255));
	box.setOutlineColor(sf::Color::Black);
	text.setFont(font);
	text.setCharacterSize(character_size);
	//text.setColor(color);
	text.setFillColor(color);
	//tick_on = false;
	this->selected = selected;
	
	box.setPosition(sf::Vector2f(x, y));
	text.setPosition(
		(int)(box.getPosition().x + (box.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f),
		(int)(box.getPosition().y)
	);
	
	if (selected)
	{
		text.setString("|");
		tick_on = true;
	}
	else
	{
		tick_on = false;
		text.setString(text_string.str());
	}

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(3, false);

	Renderer::RenderObject shape_renderable = Renderer::RenderObject(&box, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	shape_renderable.has_global_bounds = true;

	Renderer::Instance()->add_ui(shape_renderable);
	Renderer::Instance()->add_ui(Renderer::RenderObject(&text, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer()));
}

InputBox::~InputBox()
{
	Renderer::Instance()->remove_ui(&text);
	Renderer::Instance()->remove_ui(&box);
}

void InputBox::update_sfml(sf::Event sfEvent)
{
	if (sfEvent.type != sf::Event::TextEntered)
	{
		return;
	}
	if (selected)
	{
		int char_typed = sfEvent.text.unicode;
		if (char_typed < 128)
		{
			input_logic(char_typed);
		}
	}
}

void InputBox::update()
{
	if (Input::Instance()->get_mouse_down())
	{
		if (Renderer::Instance()->is_top_ui(&box))
		{
			if (box.getGlobalBounds().contains(Input::Instance()->mouse_position(get_view())) && !selected)
				set_selected(true);
			else if (!box.getGlobalBounds().contains(Input::Instance()->mouse_position(get_view())) && selected)
				set_selected(false);
		}
	}

	if (selected)
	{
		text.setPosition(
			(int)(box.getPosition().x + (box.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f),
			(int)(text.getPosition().y)
		);
		if (tick_delta >= tick_timer)
		{
			tick_delta = 0;
			if (tick_on)
			{
				tick_on = false;
				text.setString(text_string.str());

			}
			else
			{
				tick_on = true;
				text.setString(text_string.str() + "|");
			}
		}
		else
			tick_delta += Time::Instance()->delta_time();

	}

}

void InputBox::add_to_buffer(sf::View* view)
{
	set_view(view);
	//Renderer::add(Renderer::RenderObject(&text, render, SortingLayer::UI, 3, &view));
	//Renderer::add(Renderer::RenderObject(&box, render, SortingLayer::UI, 0, &view));
}

void InputBox::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	box.setPosition(sf::Vector2f(x, y));
	text.setPosition(
		(int)(box.getPosition().x + (box.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f),
		(int)(box.getPosition().y)
	);
}

void InputBox::set_selected(bool sel)
{
	Input::Instance()->set_using_input_box(sel);
	selected = sel;

	if (!selected)
	{
		text.setString(text_string.str());
		tick_on = false;
		tick_delta = 0;
	}
	else
	{
		text.setString(text_string.str() + "|");
		tick_on = true;
		tick_delta = 0;
	}
	/*
	if (!sel && tick_on)
		delete_last_char();

	tick_on = sel;
	tick_delta = 0;
	*/
}

std::string InputBox::get_text()
{
	return text_string.str();
}

float InputBox::get_text_value()
{
	try
	{
		if (std::stof(text_string.str()))
			return std::stof(text_string.str());
	}
	catch (...)
	{

		return 0.0f;
	}
}

void InputBox::set_text(std::string value)
{
	text_string.str("");
	text_string << value;
	text.setString(text_string.str());

	text.setPosition(
		(int)(box.getPosition().x + (box.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f),
		(int)(text.getPosition().y)
	);
}

const bool InputBox::is_selected() const
{
	return selected;
}

void InputBox::input_logic(int char_typed)
{
	if (char_typed != DELETE_KEY && char_typed != ENTER_KEY && char_typed != ESCAPE_KEY)
		//&& text_string.str().size() < limit)
		text_string << static_cast<char>(char_typed);
	else if (char_typed == DELETE_KEY)
	{
		if (text_string.str().length() > 0)
		{
			delete_last_char();
		}
	}
	else if (char_typed == ENTER_KEY || char_typed == ESCAPE_KEY)
	{
		set_selected(false);
		//selected
	}
	if (tick_on)
		text.setString(text_string.str() + "|");
	else
		text.setString(text_string.str());

}

void InputBox::delete_last_char()
{
	std::string t = text_string.str();
	std::string new_t = "";
	for (int i = 0; i < t.length() - 1; i++)
		new_t += t[i];

	text_string.str("");
	text_string << new_t;

	text.setString(text_string.str());

}

#pragma endregion

#pragma region LABEL

Label::Label(float x, float y, float character_size,
	sf::Font& font, std::string text, sf::Color color)
{
	this->position = sf::Vector2f(x, y);
	text_content.setFont(font);
	text_content.setCharacterSize(character_size);
	text_content.setString(text);
	text_content.setFillColor(color);
	text_content.setPosition(sf::Vector2f(x, y));

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(3, false);
	Renderer::Instance()->add_ui(Renderer::RenderObject(&text_content, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer()));
}

Label::~Label()
{
	Renderer::Instance()->remove_ui(&text_content);
}

void Label::add_to_buffer(sf::View* view)
{
	set_view(view);
	//Renderer::add(Renderer::RenderObject(&text_content, render, SortingLayer::UI, 3, &view));
}

void Label::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	text_content.setPosition(sf::Vector2f(x, y));
}

void Label::set_text(std::string new_text)
{
	text_content.setString(new_text);
}

#pragma endregion

#pragma region PANEL

Panel::Panel(float x, float y, float width, float height, sf::Color color)
{
	this->position = sf::Vector2f(x, y);
	panel_shape.setSize(sf::Vector2f(width, height));
	panel_shape.setPosition(x, y);
	panel_shape.setFillColor(color);
	panel_shape.setOutlineThickness(1.f);
	panel_shape.setOutlineColor(sf::Color::Cyan);

	active = false;

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(0, false);

	Renderer::RenderObject shape_renderable = Renderer::RenderObject(&panel_shape, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	shape_renderable.has_global_bounds = true;

	Renderer::Instance()->add_ui(shape_renderable);
	
}

Panel::~Panel()
{
	Renderer::Instance()->remove_ui(&panel_shape);
	clear();
	
}

const bool Panel::is_active() const
{
	return active;
}

void Panel::toggle_active()
{
	active = !active;
}

void Panel::update()
{
	if ( get_view() )
	{
		if (Input::Instance()->get_mouse_scroll_delta() < 0 &&
			panel_shape.getGlobalBounds().contains(Input::Instance()->mouse_position()))
			get_view()->move(0, 5000 * Time::Instance()->delta_time());
		else if (Input::Instance()->get_mouse_scroll_delta() > 0 &&
			panel_shape.getGlobalBounds().contains(Input::Instance()->mouse_position()))
			get_view()->move(0, -5000 * Time::Instance()->delta_time());
	}
	for (auto& c : content)
		c.second->update();
}

void Panel::add_to_buffer(sf::View* view)
{
	set_view(view);
	for (auto& c : content)
		c.second->set_view(view);
}

void Panel::set_position(float x, float y)
{
	GUIObject::set_position(x, y);
	panel_shape.setPosition(x, y);
	for (auto& c : content)
		c.second->set_position(x + c.second->get_position().x, y + c.second->get_position().y);
		
}

void Panel::update_sfml(sf::Event sfEvent)
{
	for (auto& c : content)
		c.second->update_sfml(sfEvent);
}

void Panel::set_render(bool render)
{
	IRenderable::set_render(render);
	for (auto& c : content)
		c.second->set_render(render);
}

void Panel::apply_texture(sf::Texture* texture, sf::IntRect rect)
{
	panel_shape.setTexture(texture);
	panel_shape.setTextureRect(rect);
	panel_shape.setFillColor(sf::Color::White);
}

void Panel::add_element(std::string key, GUIObject* gui_object)
{
	gui_object->set_position(position.x + gui_object->get_position().x, position.y + gui_object->get_position().y);
	gui_object->set_view( get_view() );
	gui_object->set_render( get_render() );
	
	content[key] = gui_object;
}

void Panel::remove_element(std::string key)
{
	std::unordered_map<std::string, GUIObject*>::iterator iter = content.find(key);
	if (iter != content.end())
		delete iter->second;
	content.erase(key);
}

std::unordered_map<std::string, GUIObject*> Panel::get_content()
{
	return content;
}


void Panel::display_mouse_pos(sf::Font& font)
{
	Debug::Instance()->mouse_position_display(font, get_view(), position);
}

GUIObject* Panel::get_element(std::string key)
{
	return content.at(key);
}

DropDownList* Panel::get_dropdown(std::string key)
{
	return dynamic_cast<DropDownList*>(content.at(key));
}

InputBox* Panel::get_inputbox(std::string key)
{
	return dynamic_cast<InputBox*>(content.at(key));
}

Checkbox* Panel::get_checkbox(std::string key)
{
	return dynamic_cast<Checkbox*>(content.at(key));
}

bool Panel::mouse_in_bounds()
{
	return panel_shape.getGlobalBounds().contains(Input::Instance()->mouse_position( get_view() ));
}

Button* Panel::get_button(std::string key)
{
	return dynamic_cast<Button*>(content.at(key));
}

Panel* Panel::get_panel(std::string key)
{
	return dynamic_cast<Panel*>(content.at(key));
}

void Panel::add_scroll_view(std::string key, ScrollView* sv)
{
	sv->set_position(position.x + sv->get_position().x, 
		position.y + sv->get_position().y);
	content[key] = sv;
	set_view(&sv->get_scroll_view());
	
}

float Panel::get_width()
{
	return panel_shape.getSize().x;
}

float Panel::get_height()
{
	return panel_shape.getSize().y;
}


void Panel::clear()
{
	for (auto& c : content)
	{
		delete c.second;
	}
	content.clear();
}


#pragma endregion

#pragma region SLIDER

Slider::Slider()
{
}

Slider::Slider(float x, float y, float width, float min_value, float max_value, 
	float default_value, bool whole_numbers)
{
	set_position(x, y);
	shape.setPosition(position);
	shape.setSize(Vector2f(width, SLIDER_HEIGHT));
	shape.setOutlineThickness(1.f);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Black);

	slider_back.setPosition(position.x, position.y + 12);
	slider_back.setSize(Vector2f(width, SLIDER_HEIGHT / 4));
	slider_back.setOutlineThickness(1.f);
	slider_back.setFillColor(sf::Color(180, 180, 180, 255));
	slider_back.setOutlineColor(sf::Color::Black);

	this->width = width;
	this->min_val = min_value;
	this->max_val = max_value;
	this->current_val = default_value;
	this->whole_numbers = whole_numbers;

	float perc = default_value / (max_value - min_value);
	slider.setPosition( x + ((width * perc) - (slider.getSize().x / 2)), y - 5);
	slider.setFillColor(sf::Color::Black);
	slider.setSize(Vector2f(SLIDER_HEIGHT / 1.5, SLIDER_HEIGHT + 10));
	slider.setOutlineThickness(1.f);
	slider.setOutlineColor(sf::Color::White);

	set_sorting_layer(Sorting::Layer::UI, false);
	Renderer::Instance()->add_ui(Renderer::RenderObject(&slider_back, get_render(), get_sorting_layer(), get_z_order(), get_view_pointer()));
	Renderer::Instance()->add_ui(Renderer::RenderObject(&slider, get_render(), get_sorting_layer(), get_z_order(), get_view_pointer()));

}

Slider::~Slider()
{
	Renderer::Instance()->remove_ui(&slider_back);
	Renderer::Instance()->remove_ui(&slider);
}

void Slider::update()
{
	if (mouse_on_slider() && Input::Instance()->get_mouse())
		held = true;
	if (Input::Instance()->get_mouse_up())
		held = false;

	if (!held)
		return;

	if (held)
	{
		float x = Input::Instance()->mouse_position(get_view()).x;

		if (x < position.x)
			x = position.x;
		if (x > position.x + width)
			x = position.x + width;

		float perc = (x - position.x) / width;
		current_val = (max_val - min_val) * perc;

		if (whole_numbers)
		{
			current_val = std::round(current_val);
		}

		slider.setPosition(x-8, position.y - 5);
		return;
	}
	
	if (mouse_in_bounds() && Input::Instance()->get_mouse_down())
	{
		float x = std::max(Input::Instance()->mouse_position(get_view()).x, position.x);
		x = std::min(x, position.x + width);

		float perc = (x - position.x) / width;
		current_val = (max_val - min_val) * perc;

		if (whole_numbers)
		{
			current_val = std::round(current_val);
		}

		slider.setPosition(x-8, position.y - 5);
		
	}
	
}

bool Slider::mouse_in_bounds()
{
	return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(
		Input::Instance()->mouse_position(get_view())));
}

bool Slider::mouse_on_slider()
{
	return slider.getGlobalBounds().contains(static_cast<sf::Vector2f>(
		Input::Instance()->mouse_position(get_view())));
}

const float Slider::get_value() const
{
	return current_val;
}

const float Slider::get_width() const
{
	return width;
}


#pragma endregion

#pragma region TRANSFORM_MOVER

TransformMover::TransformMover(Vector2f position, Vector2f size)
{
	this->position = position;
	this->size = size;
	
	horizontal_arrow.at(0) = FloatConvex::Polygon
	(
		position + Vector2f(1.f / 2.f * size.y, 0),
		{ 
			Vector2f(0,0), Vector2f(2.f / 3.f * size.x , 0),
			Vector2f(2.f / 3.f * size.x, 1.f / 2.f * size.y), Vector2f(0, 1.f / 2.f * size.y)
		}
	);
	
	horizontal_arrow.at(1) = FloatConvex::Polygon
	(
		position + Vector2f(1.f / 2.f * size.y, 0) + Vector2f(2.f / 3.f * size.x, -1.f / 4.f * size.y),
		{
			Vector2f(0, 0), Vector2f(1.f / 3.f * size.x, 1.f / 2.f * size.y), Vector2f(0, size.y)
		}
	);

	//horizontal_arrow = FloatConvex::Polygon(position, {Vector2f(0,0), Vector2f(size.x, 0), Vector2f(0, size.y)});
	
	vertical_arrow.at(0) = FloatConvex::Polygon
	(
		position,
		{
			Vector2f(0, 0), Vector2f(0, -2.f / 3.f * size.x), 
			Vector2f(1.f / 2.f * size.y, - 2.f / 3.f * size.x), Vector2f(1.f / 2.f * size.y, 0)
		}
	);
	vertical_arrow.at(1) = FloatConvex::Polygon
	(
		position + Vector2f(-1.f / 4.f * size.y, -2.f / 3.f * size.x),
		{
			Vector2f(0, 0), Vector2f(1.f / 2.f * size.y, -1.f / 3.f * size.x), Vector2f(size.y, 0)
		}
	);

	full_movement = FloatConvex::Polygon
	(
		position + Vector2f(1.f / 2.f * size.y, 0),
		{ 
			Vector2f(0, 0), Vector2f(0, -30),
			Vector2f(30, -30), Vector2f(30, 0) 
		}
	);

	horizontal_arrow.at(0).setFillColor(sf::Color::Green);
	horizontal_arrow.at(1).setFillColor(sf::Color::Green);
	vertical_arrow.at(0).setFillColor(sf::Color::Red);
	vertical_arrow.at(1).setFillColor(sf::Color::Red);
	full_movement.setFillColor(sf::Color(255, 255, 255, 50));
	full_movement.setOutlineThickness(2.f);

	set_render(true);
	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(5, false);

	Renderer::RenderObject vertical_renderable = Renderer::RenderObject(&vertical_arrow.at(0), get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	Renderer::RenderObject vertical_renderable2 = Renderer::RenderObject(&vertical_arrow.at(1), get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	vertical_renderable2.has_global_bounds = true;

	Renderer::RenderObject horizontal_renderable = Renderer::RenderObject(&horizontal_arrow.at(0), get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	Renderer::RenderObject horizontal_renderable2 = Renderer::RenderObject(&horizontal_arrow.at(1), get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	horizontal_renderable2.has_global_bounds = true;

	Renderer::RenderObject full_movement_renderable = Renderer::RenderObject(&full_movement, get_render(), get_sorting_layer(),
		get_z_order(), get_view_pointer());
	full_movement_renderable.has_global_bounds = true;

	Renderer::Instance()->add_ui(vertical_renderable);
	Renderer::Instance()->add_ui(vertical_renderable2);
	Renderer::Instance()->add_ui(horizontal_renderable);
	Renderer::Instance()->add_ui(horizontal_renderable2);
	Renderer::Instance()->add_ui(full_movement_renderable);
}

TransformMover::~TransformMover()
{
	Renderer::Instance()->remove_ui(&vertical_arrow.at(0));
	Renderer::Instance()->remove_ui(&vertical_arrow.at(1));
	Renderer::Instance()->remove_ui(&horizontal_arrow.at(0));
	Renderer::Instance()->remove_ui(&horizontal_arrow.at(1));
	Renderer::Instance()->remove_ui(&full_movement);
}

void TransformMover::update()
{
	if (Input::Instance()->get_mouse_down())
	{
		if (horizontal_arrow.at(0).contains_point(Input::Instance()->mouse_position(get_view())))
		{
			horizontal_held = true;
			offset = horizontal_arrow.at(0).get_position() - Vector2f(1.f / 2.f * size.y, 0) - Input::Instance()->mouse_position(get_view());
		}
		else if (horizontal_arrow.at(1).contains_point(Input::Instance()->mouse_position(get_view()))) 
		{
			horizontal_held = true;
			offset = horizontal_arrow.at(0).get_position() - Vector2f(1.f / 2.f * size.y, 0) - Input::Instance()->mouse_position(get_view());
		}
		
		if (vertical_arrow.at(0).contains_point(Input::Instance()->mouse_position(get_view())))
		{
			vertical_held = true;
			offset = vertical_arrow.at(0).get_position() - Input::Instance()->mouse_position(get_view());
		}
		else if (vertical_arrow.at(1).contains_point(Input::Instance()->mouse_position(get_view())))
		{
			vertical_held = true;
			offset = vertical_arrow.at(0).get_position() - Input::Instance()->mouse_position(get_view());
		}

		if (full_movement.contains_point(Input::Instance()->mouse_position(get_view())))
		{
			vertical_held = true;
			horizontal_held = true;
			offset = full_movement.get_position() - Vector2f(1.f / 2.f * size.y, 0) - Input::Instance()->mouse_position(get_view());
			
		}
	}
	if (Input::Instance()->get_mouse_up())
	{
		horizontal_held = false;
		vertical_held = false;
		offset = Vector2f::Zero();
	}

	if (horizontal_held)
	{
		position.x = Input::Instance()->mouse_position(get_view()).x + offset.x;
	}
	if (vertical_held)
	{
		position.y = Input::Instance()->mouse_position(get_view()).y + offset.y;
	}
	if (vertical_held || horizontal_held)
	{
		vertical_arrow.at(0).set_position(position);
		vertical_arrow.at(1).set_position(position + Vector2f(-1.f / 4.f * size.y, -2.f / 3.f * size.x));
		
		horizontal_arrow.at(0).set_position(position + Vector2f(1.f / 2.f * size.y, 0));
		horizontal_arrow.at(1).set_position(position + Vector2f(1.f / 2.f * size.y, 0) + Vector2f(2.f / 3.f * size.x, -1.f / 4.f * size.y));

		full_movement.set_position(position + Vector2f(1.f / 2.f * size.y, 0));
	}
	

}

void TransformMover::set_position(float x, float y)
{
	position = Vector2f(x, y);

	vertical_arrow.at(0).set_position(position);
	vertical_arrow.at(1).set_position(position + Vector2f(-1.f / 4.f * size.y, -2.f / 3.f * size.x));
	horizontal_arrow.at(0).set_position(position + Vector2f(1.f / 2.f * size.y, 0));
	horizontal_arrow.at(1).set_position(position + Vector2f(1.f / 2.f * size.y, 0) + Vector2f(2.f / 3.f * size.x, -1.f / 4.f * size.y));
	full_movement.set_position(position + Vector2f(1.f / 2.f * size.y, 0));
}

bool TransformMover::mouse_in_bounds()
{
	Vector2f pos = Input::Instance()->mouse_position(get_view());
	return 
		horizontal_arrow.at(0).getGlobalBounds().contains(pos) ||
		horizontal_arrow.at(1).getGlobalBounds().contains(pos) || 
		vertical_arrow.at(0).getGlobalBounds().contains(pos) ||
		vertical_arrow.at(1).getGlobalBounds().contains(pos) ||
		full_movement.getGlobalBounds().contains(pos);
}

sf::Vector2f TransformMover::get_position()
{
	return position;
}


#pragma endregion

#pragma region PROGRESS_BAR

ProgressBar::ProgressBar()
{
	set_sorting_layer(Sorting::Layer::UI);
	set_z_order(0);
	set_render(true);

	full_state_shape.setFillColor(sf::Color::Transparent);

	Renderer::Instance()->add_ui(Renderer::RenderObject(&progress_shape, this));
	Renderer::Instance()->add_ui(Renderer::RenderObject(&full_state_shape, this));
}

ProgressBar::~ProgressBar()
{
	Renderer::Instance()->remove_ui(&progress_shape);
	Renderer::Instance()->remove_ui(&full_state_shape);
}

void ProgressBar::set_position(Vector2f position)
{
	if (this->position != position)
	{
		this->position = position;
		progress_shape.set_position(position);
		full_state_shape.set_position(position);
	}
}

void ProgressBar::set_size(Vector2f size)
{
	if (this->size == size)
	{
		this->size = size;
		progress_shape = FloatConvex::Polygon(
			position,
			{ Vector2f(0,0), Vector2f(size.x * percentage, 0), Vector2f(size.x * percentage, size.y), Vector2f(0, size.y) }
		);
		full_state_shape = FloatConvex::Polygon(
			position,
			{ Vector2f(0,0), Vector2f(size.x, 0), Vector2f(size.x, size.y), Vector2f(0, size.y) }
		);
	}
}

void ProgressBar::set_color(sf::Color color)
{
	progress_shape.setFillColor(color);
}

void ProgressBar::set_percentage(float percentage)
{
	if (this->percentage != percentage)
	{
		this->percentage = percentage;
		set_size(size);
	}
}

#pragma endregion





}