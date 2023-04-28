#include "pch.h"
#include "TextureSelector.h"
#include "core/ResourceManager.h"
#include "core/Renderer.h"
#include "core/Input.h"
#include "core/Time.h" 
#include "Gui.h"

namespace bm98::GUI
{
using namespace core;
TextureSelector::TextureSelector(float x, float y, float width, float height,
	std::vector<std::string> tile_sets)
{
	this->font = &ResourceManager::Instance()->get_font("calibri-regular.ttf");
	this->position = sf::Vector2f(x, y);


	texture_selector_panel = new GUI::Panel(x, y, width + 20, height + 20);
	sheet_panel = new GUI::Panel(x + 20, y + 60, width - 20, height - 60);

	sheet_default_pos = Vector2f(x + 20, y + 60);
	this->sheet.setPosition(sheet_default_pos);

	set_sorting_layer(Sorting::Layer::UI, false);
	set_z_order(0, false);
	set_render(false);

	init_view();

	float left = this->sheet_panel->get_position().x / Renderer::Instance()->get_window_size().x;
	float top = this->sheet_panel->get_position().y / Renderer::Instance()->get_window_size().y;
	float w = this->sheet_panel->get_width() / Renderer::Instance()->get_window_size().x;
	float h = this->sheet_panel->get_height() / Renderer::Instance()->get_window_size().y;

	this->texture_view->setViewport(sf::FloatRect(left, top, w, h));

	Renderer::Instance()->add_ui(Renderer::RenderObject(&sheet, this));
	Renderer::Instance()->add_ui(Renderer::RenderObject(&selector, this));

	init_dropdowns(tile_sets);
	init_checkboxes(20);


	texture_selector_panel->set_render(get_render());
	sheet_panel->set_render(get_render());

	//init(x, y, width, height, grid_size, texture_sheet, tile_sets);	
}

TextureSelector::TextureSelector()
{

}

TextureSelector::~TextureSelector()
{
	Renderer::Instance()->remove_ui(&sheet);
	Renderer::Instance()->remove_ui(&selector);

	delete texture_selector_panel;
	delete sheet_panel;
	delete texture_view;
}

void TextureSelector::init(float grid_size, const sf::Texture* texture_sheet)
{
	active = true;
	this->grid_size = grid_size;

	this->sheet.setTexture(*texture_sheet);

	this->selector.setPosition(get_position());
	this->selector.setSize(sf::Vector2f(grid_size, grid_size));
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineThickness(1.f);
	this->selector.setOutlineColor(sf::Color::Cyan);


	this->texture_rect.width = static_cast<int>(grid_size);
	this->texture_rect.height = static_cast<int>(grid_size);

}

void TextureSelector::set_inactive()
{
	active = false;
	set_render(false);
	texture_selector_panel->set_render(false);
	sheet_panel->set_render(false);
}

const bool& TextureSelector::mouse_in_bounds()
{
	return (get_render() && sheet_panel->mouse_in_bounds());
}

const bool& TextureSelector::mouse_in_container()
{
	return (get_render() && texture_selector_panel->mouse_in_bounds());
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

const Sorting::Layer TextureSelector::get_layer() const
{
	return (Sorting::Layer)texture_selector_panel->get_dropdown("layer_selector")->get_selected_index();
}

void TextureSelector::toggle_hidden()
{
	if (!active)
		return;
	set_render(!get_render());
	texture_selector_panel->set_render(get_render());
	sheet_panel->set_render(get_render());
}

void TextureSelector::init_view()
{
	texture_view = new sf::View();

	texture_view->setSize
	(
		500.f, 500.f
	);

	texture_view->setCenter(sheet_default_view_pos);

	set_view(texture_view);
}

void TextureSelector::init_dropdowns(std::vector<std::string> tilesets_names)
{
	std::vector<std::string> big = Sorting::ToVector();
	std::vector<std::string> sub = { big.begin(), big.end() - 1 };
	GUI::DropDownList* layer_selector = new GUI::DropDownList(0.f, 0.f, 120.f, 50.f, *font, sub);
	GUI::DropDownList* tile_collection_selector = new GUI::DropDownList(130, 0, 120.f, 50.f, *font, tilesets_names);
	GUI::DropDownList* tile_type_selector = new GUI::DropDownList(260, 0, 120, 50, *font, TileNS::ToVector());

	texture_selector_panel->add_element("layer_selector", layer_selector);
	texture_selector_panel->add_element("tile_collection_selector", tile_collection_selector);
	texture_selector_panel->add_element("tile_type_selector", tile_type_selector);

}

void TextureSelector::init_checkboxes(float size)
{
	GUI::Checkbox* collision_checkbox = new GUI::Checkbox(400, 15, size);
	GUI::Checkbox* animation_checkbox = new GUI::Checkbox(430, 15, size);

	texture_selector_panel->add_element("collision_checkbox", collision_checkbox);
	texture_selector_panel->add_element("animation_checkbox", animation_checkbox);
}

void TextureSelector::set_texture_sheet(sf::Texture* texture)
{
	sf::Sprite sheet;
	sheet.setTexture(*texture);
	sheet.setPosition(sheet_default_pos);
	this->texture_view->setCenter(sheet_default_view_pos);
	
	this->sheet = sheet;
}

void TextureSelector::set_texture_rect()
{
	mouse_pos_grid.x = (Input::Instance()->mouse_position(get_view()).x - static_cast<int>(sheet_panel->get_position().x))
		/ static_cast<unsigned>(grid_size);
	mouse_pos_grid.y = (Input::Instance()->mouse_position(get_view()).y - static_cast<int>(sheet_panel->get_position().y))
		/ static_cast<unsigned>(grid_size);

	if (Input::Instance()->get_mouse_down(Input::Mouse::LEFT))
	{
		texture_rect.width = grid_size;
		texture_rect.height = grid_size;
		selector.setPosition(
			sheet_panel->get_position().x + mouse_pos_grid.x * grid_size,
			sheet_panel->get_position().y + mouse_pos_grid.y * grid_size
		);
		selector.setSize(sf::Vector2f(grid_size, grid_size));
		texture_rect.left = static_cast<int>(selector.getPosition().x - sheet_panel->get_position().x);
		texture_rect.top = static_cast<int>(selector.getPosition().y - sheet_panel->get_position().y);

		return;
	}

	// HANDLE DRAGGING SELECTOR TO RIGHT
	if (texture_rect.left < (static_cast<int>((sheet_panel->get_position().x + mouse_pos_grid.x * grid_size) - sheet_panel->get_position().x)) &&
		texture_rect.width != (static_cast<int>((sheet_panel->get_position().x + mouse_pos_grid.x * grid_size) - sheet_panel->get_position().x)) - texture_rect.left + grid_size)
	{
		if (texture_rect.width < (static_cast<int>((sheet_panel->get_position().x + mouse_pos_grid.x * grid_size) - sheet_panel->get_position().x)) - texture_rect.left + grid_size)
		{
			selector.setSize(sf::Vector2f(selector.getSize().x + grid_size, selector.getSize().y));
			texture_rect.width += static_cast<int>(grid_size);
		}
		else
		{
			selector.setSize(sf::Vector2f(selector.getSize().x - grid_size, selector.getSize().y));
			texture_rect.width -= static_cast<int>(grid_size);
		}

		texture_rect.left = static_cast<int>(selector.getPosition().x - sheet_panel->get_position().x);
		texture_rect.top = static_cast<int>(selector.getPosition().y - sheet_panel->get_position().y);
	}
	// HANDLE DRAGGING SELECTOR TO BOTTOM
	if (texture_rect.top < (static_cast<int>((sheet_panel->get_position().y + mouse_pos_grid.y * grid_size) - sheet_panel->get_position().y)) &&
		texture_rect.height != (static_cast<int>((sheet_panel->get_position().y + mouse_pos_grid.y * grid_size) - sheet_panel->get_position().y)) - texture_rect.top + grid_size)
	{
		if (texture_rect.height < (static_cast<int>((sheet_panel->get_position().y + mouse_pos_grid.y * grid_size) - sheet_panel->get_position().y)) - texture_rect.top + grid_size)
		{
			selector.setSize(sf::Vector2f(selector.getSize().x, selector.getSize().y + grid_size));
			texture_rect.height += static_cast<int>(grid_size);
		}
		else
		{
			selector.setSize(sf::Vector2f(selector.getSize().x, selector.getSize().y - grid_size));
			texture_rect.height -= static_cast<int>(grid_size);
		}
		texture_rect.left = static_cast<int>(selector.getPosition().x - sheet_panel->get_position().x);
		texture_rect.top = static_cast<int>(selector.getPosition().y - sheet_panel->get_position().y);

	}
}

DropDownList* TextureSelector::get_layer_dropdown()
{
	return texture_selector_panel->get_dropdown("layer_selector");
}

DropDownList* TextureSelector::get_tiletype_selector()
{
	return texture_selector_panel->get_dropdown("tile_type_selector");
}

DropDownList* TextureSelector::get_tileset_selector()
{
	return  texture_selector_panel->get_dropdown("tile_collection_selector");
}

Checkbox* TextureSelector::get_collision_checkbox()
{
	return texture_selector_panel->get_checkbox("collision_checkbox");
}

Checkbox* TextureSelector::get_animation_checkbox()
{
	return texture_selector_panel->get_checkbox("animation_checkbox");
}

void TextureSelector::update()
{
	if (!get_render())
		return;

	texture_selector_panel->get_checkbox("animation_checkbox")->update();
	texture_selector_panel->get_checkbox("collision_checkbox")->update();

	texture_selector_panel->get_dropdown("layer_selector")->update();
	texture_selector_panel->get_dropdown("tile_type_selector")->update();
	texture_selector_panel->get_dropdown("tile_collection_selector")->update();

	float delta = Time::Instance()->delta_time();

	if (Input::Instance()->get_action("SHIFT") && Input::Instance()->get_mouse_scroll_delta() < 0)
	{
		texture_view->move(20000 * delta, 0);
	}
	else if (Input::Instance()->get_action("SHIFT") && Input::Instance()->get_mouse_scroll_delta() > 0)
	{
		texture_view->move(-20000 * delta, 0);
	}
	else if (Input::Instance()->get_mouse_scroll_delta() < 0)
	{
		texture_view->move(0, 20000 * delta);
	}
	else if (Input::Instance()->get_mouse_scroll_delta() > 0)
	{
		texture_view->move(0, -20000 * delta);
	}
	/*
	*  here need to change how this works entirely
	*
	* instead of updating texture_rect every update, only update on mouse release
	*/

}


void TextureSelector::add_to_buffer(sf::View* view)
{
	//text_view = view;

	//set_render(!hidden);
	texture_selector_panel->get_checkbox("animation_checkbox")->add_to_buffer();
	texture_selector_panel->get_checkbox("collision_checkbox")->add_to_buffer();

	texture_selector_panel->get_dropdown("layer_selector")->add_to_buffer();
	texture_selector_panel->get_dropdown("tile_type_selector")->add_to_buffer();
	texture_selector_panel->get_dropdown("tile_collection_selector")->add_to_buffer();

}

void TextureSelector::clear()
{
	set_render(false);

	delete texture_selector_panel;
	delete sheet_panel;


	if (this->sheet.getGlobalBounds().width > this->sheet_panel->get_width())
	{
		this->sheet.setTextureRect(sf::IntRect(0, 0, 0, 0));
	}
	if (this->sheet.getGlobalBounds().height > this->sheet_panel->get_height())
	{
		this->sheet.setTextureRect(sf::IntRect(0, 0, 0 ,0));
	}

	this->selector.setSize(sf::Vector2f(0, 0));
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineThickness(0.f);

	this->texture_rect.width = 0;
	this->texture_rect.height = 0;

	//this->texture_view = texture_view;

	// following two need reset to use text_view
	Renderer::Instance()->remove(&sheet);
	Renderer::Instance()->remove(&selector);

}

}