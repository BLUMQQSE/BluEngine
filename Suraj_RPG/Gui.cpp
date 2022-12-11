#include "pch.h"
#include "Gui.h"
#include "Time.h"
#include "Input.h"
#include "Renderer.h"
#include "Debug.h"

namespace bm98::GUI
{
using namespace core;

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
		shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height /*/ 2.f*/
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
}

Button::~Button()
{
}

void Button::update()
{
	if (shape.getGlobalBounds().contains(Input::mouse_position(view)) &&
		Input::get_mouse_down(Input::Mouse::LEFT))
	{
		button_state = ButtonState::BTN_PRESSED;
	}
	else if (shape.getGlobalBounds().contains(Input::mouse_position(view)))
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

void Button::add_to_buffer(sf::View* view)
{
	Renderer::add(Renderer::RenderObject(&shape, SortingLayer::UI, id + 10, view));
	Renderer::add(Renderer::RenderObject(&text, SortingLayer::UI, id + 10, view));
}

void Button::set_position(float x, float y)
{
	GUIObject::set_position(x, y);
	this->shape.setPosition(sf::Vector2f(x, y));

	this->text.setPosition(
		shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height /*/ 2.f*/
	);
	
}

const bool Button::is_pressed() const
{
	return button_state == ButtonState::BTN_PRESSED;
}

const bool Button::mouse_in_bounds() const
{
	return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(Input::mouse_position(view)));
}

const std::string Button::get_text() const
{
	return text.getString();
}

const short unsigned& Button::get_id() const
{
	return id;
}

void Button::set_pressed()
{
	button_state = ButtonState::BTN_PRESSED;
}

void Button::set_text(const std::string text)
{
	this->text.setString(text);
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
		show_list = !show_list;
	}

	if (show_list)
	{
		for (auto& i : list)
		{
			i->update();
			if (i->is_pressed())
			{
				show_list = false;
				selection_change = true;
				active_selection->set_text(i->get_text());
				active_selection->set_id(i->get_id());
			}
			else if (Input::get_mouse_down(Input::Mouse::LEFT) && !active_selection->is_pressed())
			{
				show_list = false;
			}
		}
	}
}


void DropDownList::add_to_buffer(sf::View* view)
{
	active_selection->add_to_buffer(view);
	if (show_list)
	{
		for (auto& i : list)
		{
			i->add_to_buffer(view);
		}
	}
}

void DropDownList::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	active_selection->set_position(x, y);
	for (size_t i = 0; i < list.size(); i++)
		list[i]->set_position(x, y + ((i + 1) * height));
		
	
}

void DropDownList::set_view(sf::View* view)
{
	active_selection->set_view(view);
	for (auto& b : list)
		b->set_view(view);
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

const short unsigned& DropDownList::get_selected_index() const
{
	return active_selection->get_id();
}

const Button* DropDownList::get_selected_button() const
{
	return active_selection;
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
	checked = false;
}

Checkbox::~Checkbox()
{
}

void Checkbox::update()
{
	if (box.getGlobalBounds().contains(Input::mouse_position(view)) &&
		Input::get_mouse_down(Input::Mouse::LEFT))
	{
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
	Renderer::add(Renderer::RenderObject(&box, SortingLayer::UI, 1, view));
	Renderer::add(Renderer::RenderObject(&check, SortingLayer::UI, 1, view));
}

void Checkbox::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	float check_offset = size * .2f;
	box.setPosition(x, y);
	check.setPosition(x + check_offset, y + check_offset);
}

bool Checkbox::is_checked()
{
	return checked;
}

#pragma endregion


#pragma region ScrollView

ScrollView::ScrollView(float x, float y, float width, float height, bool vertical, bool horizontal)
{
	this->position = sf::Vector2f(x, y);
	this->width = width;
	this->height = height;
	bounds.setPosition(x, y);
	bounds.setSize(sf::Vector2f(width, height));
	bounds.setOutlineThickness(1.f);
	bounds.setOutlineColor(sf::Color::White);
	bounds.setFillColor(sf::Color::Transparent);


	scroll_view.setSize(bounds.getSize().x, bounds.getSize().y);
	scroll_view.setCenter(bounds.getPosition().x + bounds.getSize().x / 2,
		bounds.getPosition().y + bounds.getSize().y / 2);

	float left = bounds.getPosition().x / Renderer::get_window_size().x;
	float top = bounds.getPosition().y / Renderer::get_window_size().y;
	float w = bounds.getSize().x / Renderer::get_window_size().x;
	float h = bounds.getSize().y / Renderer::get_window_size().y;

	scroll_view.setViewport(sf::FloatRect(left, top, w, h));

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

ScrollView::~ScrollView()
{
}

sf::View& ScrollView::get_scroll_view()
{
	return scroll_view;
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
	Renderer::add(Renderer::RenderObject(&bounds, SortingLayer::UI, 0, &scroll_view));
	if (vertical)
		Renderer::add(Renderer::RenderObject(&vertical_handle, SortingLayer::UI, 0, &scroll_view));
	if (horizontal)
		Renderer::add(Renderer::RenderObject(&horizontal_handle, SortingLayer::UI, 0, &scroll_view));
}

void ScrollView::set_position(float x, float y)
{
	GUIObject::set_position(x, y);
	bounds.setPosition(x, y);


	scroll_view.setCenter(bounds.getPosition().x + bounds.getSize().x / 2,
		bounds.getPosition().y + bounds.getSize().y / 2);

	float left = bounds.getPosition().x / Renderer::get_window_size().x;
	float top = bounds.getPosition().y / Renderer::get_window_size().y;
	float w = bounds.getSize().x / Renderer::get_window_size().x;
	float h = bounds.getSize().y / Renderer::get_window_size().y;

	scroll_view.setViewport(sf::FloatRect(left, top, w, h));

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


#pragma region TEXTURE_SELECTOR

TextureSelector::TextureSelector(float x, float y, float width, float height, float grid_size,
	const sf::Texture* texture_sheet, sf::Font& font, std::vector<std::string> tile_sets)
	: font(font)

{
	this->position = sf::Vector2f(x, y);
	this->hidden = false;
	this->container.setSize(sf::Vector2f(width + 20, height + 20));
	this->bounds.setSize(sf::Vector2f(width - 20, height - 60));
	this->container.setPosition(x, y);
	this->bounds.setPosition(x + 20, y + 60);
	this->container.setFillColor(sf::Color::Transparent);
	this->bounds.setFillColor(sf::Color(50, 50, 50, 100));
	this->container.setOutlineThickness(1.f);
	this->bounds.setOutlineThickness(1.f);
	this->container.setOutlineColor(sf::Color(255, 255, 255, 200));
	this->bounds.setOutlineColor(sf::Color(255, 255, 255, 200));

	this->grid_size = grid_size;

	this->sheet.setTexture(*texture_sheet);

	this->sheet.setPosition(x + 20, y + 60);

	if (this->sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width)
	{
		this->sheet.setTextureRect(sf::IntRect(0, 0, this->bounds.getGlobalBounds().width,
			this->sheet.getGlobalBounds().height));
	}
	if (this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height)
	{
		this->sheet.setTextureRect(sf::IntRect(0, 0, this->bounds.getGlobalBounds().width,
			this->sheet.getGlobalBounds().height));
	}

	this->selector.setPosition(x, y);
	this->selector.setSize(sf::Vector2f(grid_size, grid_size));
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineThickness(1.f);
	this->selector.setOutlineColor(sf::Color::Cyan);


	this->texture_rect.width = static_cast<int>(grid_size);
	this->texture_rect.height = static_cast<int>(grid_size);



	this->view.setSize(bounds.getSize().x, bounds.getSize().y);
	this->view.setCenter(bounds.getPosition().x + bounds.getSize().x / 2,
		bounds.getPosition().y + bounds.getSize().y / 2);

	float left = bounds.getPosition().x / Renderer::get_window_size().x;
	float top = bounds.getPosition().y / Renderer::get_window_size().y;
	float w = bounds.getSize().x / Renderer::get_window_size().x;
	float h = bounds.getSize().y / Renderer::get_window_size().y;

	this->view.setViewport(sf::FloatRect(left, top, w, h));



	init_buttons(x, y);
	init_dropdowns(x, y, tile_sets);
	init_checkboxes(x, y, 20);
}

TextureSelector::~TextureSelector()
{
	delete layer_selector;
	delete tile_collection_selector;
	delete tile_type_selector;

	delete collision_checkbox;
	delete animation_checkbox;
}

const bool& TextureSelector::mouse_in_bounds() const
{
	return (!hidden && bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(Input::mouse_position_window())));
}

const bool& TextureSelector::mouse_in_container() const
{
	return (!hidden && container.getGlobalBounds().contains(static_cast<sf::Vector2f>(Input::mouse_position_window())));
}

const sf::IntRect& TextureSelector::get_texture_rect() const
{
	return texture_rect;
}

sf::IntRect TextureSelector::convert_rect_to_grid(sf::IntRect rect)
{
	// convert texture_rect to be in grid_size units
	sf::IntRect converted_rect;
	converted_rect.left = rect.left;
	converted_rect.top = rect.top;

	converted_rect.width = rect.width;
	converted_rect.height = rect.height;

	return converted_rect;
}

const SortingLayer TextureSelector::get_layer() const
{
	return (SortingLayer)layer_selector->get_selected_index();
}

void TextureSelector::toggle_hidden()
{
	hidden = !hidden;
}

void TextureSelector::init_buttons(float x, float y)
{

}

void TextureSelector::init_dropdowns(float x, float y, std::vector<std::string> tilesets_names)
{
	//std::string list[((int)SortingLayer::UI)] = { "Background", "Item", "Actor", "Effects", "Foreground" };
	std::vector<std::string> big = Global::layers_to_vector();
	std::vector<std::string> sub = { big.begin(), big.end() - 1 };
	layer_selector = new DropDownList(x, y, 120.f, 50.f, font, sub);
	tile_collection_selector = new DropDownList(x + 130, y, 120.f, 50.f, font, tilesets_names);
	tile_type_selector = new DropDownList(x + 260, y, 120, 50, font, Global::tiletypes_to_vector());
}

void TextureSelector::init_checkboxes(float x, float y, float size)
{
	collision_checkbox = new GUI::Checkbox(x + 400, y + 15, size);
	animation_checkbox = new GUI::Checkbox(x + 430, y + 15, size);
}

void TextureSelector::set_texture_sheet(sf::Texture* texture)
{
	sf::Sprite sheet;
	sheet.setTexture(*texture);
	sheet.setPosition(this->sheet.getPosition());

	if (sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width)
	{
		sheet.setTextureRect(sf::IntRect(0, 0, this->bounds.getGlobalBounds().width,
			sheet.getGlobalBounds().height));
	}
	if (sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height)
	{
		sheet.setTextureRect(sf::IntRect(0, 0, this->bounds.getGlobalBounds().width,
			sheet.getGlobalBounds().height));
	}
	this->sheet = sheet;
}

void TextureSelector::set_texture_rect()
{
	mouse_pos_grid.x = (Input::mouse_position(&this->view).x - static_cast<int>(bounds.getPosition().x))
		/ static_cast<unsigned>(grid_size);
	mouse_pos_grid.y = (Input::mouse_position(&this->view).y - static_cast<int>(bounds.getPosition().y))
		/ static_cast<unsigned>(grid_size);

	if (Input::get_mouse_down(Input::Mouse::LEFT))
	{
		texture_rect.width = grid_size;
		texture_rect.height = grid_size;
		selector.setPosition(
			bounds.getPosition().x + mouse_pos_grid.x * grid_size,
			bounds.getPosition().y + mouse_pos_grid.y * grid_size
		);
		selector.setSize(sf::Vector2f(grid_size, grid_size));
		texture_rect.left = static_cast<int>(selector.getPosition().x - bounds.getPosition().x);
		texture_rect.top = static_cast<int>(selector.getPosition().y - bounds.getPosition().y);

		return;
	}

	// HANDLE DRAGGING SELECTOR TO RIGHT
	if (texture_rect.left < (static_cast<int>((bounds.getPosition().x + mouse_pos_grid.x * grid_size) - bounds.getPosition().x)) &&
		texture_rect.width != (static_cast<int>((bounds.getPosition().x + mouse_pos_grid.x * grid_size) - bounds.getPosition().x)) - texture_rect.left + grid_size)
	{
		if (texture_rect.width < (static_cast<int>((bounds.getPosition().x + mouse_pos_grid.x * grid_size) - bounds.getPosition().x)) - texture_rect.left + grid_size)
		{
			selector.setSize(sf::Vector2f(selector.getSize().x + grid_size, selector.getSize().y));
			texture_rect.width += static_cast<int>(grid_size);
		}
		else
		{
			selector.setSize(sf::Vector2f(selector.getSize().x - grid_size, selector.getSize().y));
			texture_rect.width -= static_cast<int>(grid_size);
		}

		texture_rect.left = static_cast<int>(selector.getPosition().x - bounds.getPosition().x);
		texture_rect.top = static_cast<int>(selector.getPosition().y - bounds.getPosition().y);
	}
	// HANDLE DRAGGING SELECTOR TO BOTTOM
	if (texture_rect.top < (static_cast<int>((bounds.getPosition().y + mouse_pos_grid.y * grid_size) - bounds.getPosition().y)) &&
		texture_rect.height != (static_cast<int>((bounds.getPosition().y + mouse_pos_grid.y * grid_size) - bounds.getPosition().y)) - texture_rect.top + grid_size)
	{
		if (texture_rect.height < (static_cast<int>((bounds.getPosition().y + mouse_pos_grid.y * grid_size) - bounds.getPosition().y)) - texture_rect.top + grid_size)
		{
			selector.setSize(sf::Vector2f(selector.getSize().x, selector.getSize().y + grid_size));
			texture_rect.height += static_cast<int>(grid_size);
		}
		else
		{
			selector.setSize(sf::Vector2f(selector.getSize().x, selector.getSize().y - grid_size));
			texture_rect.height -= static_cast<int>(grid_size);
		}
		texture_rect.left = static_cast<int>(selector.getPosition().x - bounds.getPosition().x);
		texture_rect.top = static_cast<int>(selector.getPosition().y - bounds.getPosition().y);

	}


}

DropDownList* TextureSelector::get_layer_dropdown()
{
	return layer_selector;
}

DropDownList* TextureSelector::get_tiletype_selector()
{
	return tile_type_selector;
}

DropDownList* TextureSelector::get_tileset_selector()
{
	return tile_collection_selector;
}

Checkbox* TextureSelector::get_collision_checkbox()
{
	return collision_checkbox;
}

Checkbox* TextureSelector::get_animation_checkbox()
{
	return animation_checkbox;
}

void TextureSelector::update()
{
	if (hidden)
		return;

	collision_checkbox->update();
	animation_checkbox->update();

	layer_selector->update();
	tile_type_selector->update();
	tile_collection_selector->update();

	if (Input::get_action("SHIFT") && Input::get_mouse_scroll_delta() < 0)
		view.move(2000 * Time::delta_time(), 0);
	else if (Input::get_action("SHIFT") && Input::get_mouse_scroll_delta() > 0)
		view.move(-2000 * Time::delta_time(), 0);
	else if (Input::get_mouse_scroll_delta() < 0)
		view.move(0, 2000 * Time::delta_time());
	else if (Input::get_mouse_scroll_delta() > 0)
		view.move(0, -2000 * Time::delta_time());

	/*
	*  here need to change how this works entirely
	*
	* instead of updating texture_rect every update, only update on mouse release
	*/

}


void TextureSelector::add_to_buffer(sf::View* view)
{
	if (hidden)
		return;
	collision_checkbox->add_to_buffer(view);
	animation_checkbox->add_to_buffer(view);

	layer_selector->add_to_buffer(view);
	tile_collection_selector->add_to_buffer(view);
	tile_type_selector->add_to_buffer(view);
	Renderer::add(Renderer::RenderObject(&container, SortingLayer::UI, 0, view));
	Renderer::add(Renderer::RenderObject(&bounds, SortingLayer::UI, 0, view));
	Renderer::add(Renderer::RenderObject(&sheet, SortingLayer::UI, 0, &this->view));

	Renderer::add(Renderer::RenderObject(&selector, SortingLayer::UI, 0, &this->view));
}




#pragma endregion


#pragma region InputBox

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
		box.getPosition().x + (box.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		box.getPosition().y
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
}

InputBox::~InputBox()
{

}

void InputBox::update_sfml(sf::Event sfEvent)
{
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
	if (
		Input::get_mouse_down(Input::Mouse::LEFT))
	{
		if (box.getGlobalBounds().contains(Input::mouse_position(view)))
			set_selected(true);
		else
			set_selected(false);
	}

	if (selected)
	{
		text.setPosition(
			box.getPosition().x + (box.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
			text.getPosition().y
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
			tick_delta += Time::delta_time();

	}

}

void InputBox::add_to_buffer(sf::View* view)
{
	Renderer::add(Renderer::RenderObject(&text, SortingLayer::UI, 3, view));
	Renderer::add(Renderer::RenderObject(&box, SortingLayer::UI, 0, view));
}

void InputBox::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	box.setPosition(sf::Vector2f(x, y));
	text.setPosition(
		box.getPosition().x + (box.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		box.getPosition().y
	);
}

void InputBox::set_selected(bool sel)
{
	selected = sel;

	if (!selected)
		text.setString(text_string.str());
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
	text.setString(value);
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
	else
	{
		selected = false;
		tick_on = false;
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
}

Label::~Label()
{
}

void Label::add_to_buffer(sf::View* view)
{
	Renderer::add(Renderer::RenderObject(&text_content, SortingLayer::UI, 3, view));
}

void Label::set_position(float x, float y)
{
	//GUIObject::set_position(x, y);
	text_content.setPosition(sf::Vector2f(x, y));
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
}

Panel::~Panel()
{
	for (auto& c : content)
		delete c.second;
	content.clear();
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
	if (view)
	{
		if (Input::get_mouse_scroll_delta() < 0 && 
			panel_shape.getGlobalBounds().contains(Input::mouse_position()))
			view->move(0, 5000 * Time::delta_time());
		else if (Input::get_mouse_scroll_delta() > 0 &&
			panel_shape.getGlobalBounds().contains(Input::mouse_position()))
			view->move(0, -5000 * Time::delta_time());
	}

	for (auto& c : content)
		c.second->update();
}

void Panel::add_to_buffer(sf::View* view)
{
	Renderer::add(Renderer::RenderObject(&panel_shape, SortingLayer::UI, 0, view));
	for (auto& c : content)
		c.second->add_to_buffer(this->view);
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

void Panel::add_element(std::string key, GUIObject* gui_object)
{
	gui_object->set_position(position.x + gui_object->get_position().x, position.y + gui_object->get_position().y);
	gui_object->set_view(this->view);
	
	content[key] = gui_object;
}

void Panel::remove_element(std::string key)
{
	content.erase(key);
}

std::unordered_map<std::string, GUIObject*> Panel::get_content()
{
	return content;
}

void Panel::set_view(sf::View* view)
{
	this->view = view;
	for (auto& c : content)
		c.second->set_view(view);
}

void Panel::display_mouse_pos(sf::Font& font)
{
	Debug::mouse_position_display(font, view, position);
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
	content.clear();
}


#pragma endregion





}