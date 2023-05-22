#include "pch.h"
#include "PauseMenu.h"
#include "core/Renderer.h"
#include "Time.h"
namespace bm98
{
using namespace core;

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font)
	:font(font)
{
	background.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x),
		static_cast<float>(window.getSize().y)
	));
	background.setFillColor(sf::Color(20, 20, 20, 100));

	container.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x) / 2.f,
		static_cast<float>(window.getSize().y - 60.f)
	));
	container.setFillColor(sf::Color(20, 20, 20, 200));
	container.setPosition(
		static_cast<float>(window.getSize().x) / 2.f - container.getSize().x / 2.f, 30.f);
	container.setOutlineThickness(1.f);
	container.setOutlineColor(sf::Color::White);
	menu_text.setFont(font);
	menu_text.setFillColor(sf::Color(255, 255, 255, 200));
	menu_text.setCharacterSize(30);
	menu_text.setString("Menu");
	menu_text.setPosition(container.getPosition().x + container.getSize().x / 2.f
		- menu_text.getGlobalBounds().width / 2.f
		, container.getPosition().y + 20.f);

	set_render(false);
	set_sorting_layer(Sorting::Layer::UI, false);
	Renderer::Instance()->add(Renderer::RenderObject(&background, this));
	Renderer::Instance()->add(Renderer::RenderObject(&container, this));
	Renderer::Instance()->add(Renderer::RenderObject(&menu_text, this));
}

PauseMenu::~PauseMenu()
{
	//if (get_render())
	//{
		//Time::Instance()->set_time_scale(previous_time_scale);
	//}
	Renderer::Instance()->remove(&background);
	Renderer::Instance()->remove(&container);
	Renderer::Instance()->remove(&menu_text);
	
}

std::map<std::string, std::weak_ptr<GUI::Button>> PauseMenu::get_buttons()
{
	std::map<std::string, std::weak_ptr<GUI::Button>> result;
	for (auto& b : buttons)
	{
		result[b.first] = b.second;
	}
	return result;
}

void PauseMenu::add_button(const std::string key, float x, float y, const std::string text)
{
	buttons[key] = std::make_shared<GUI::Button>(x, y, 150.f, 60.f, &font, text, 28,
		sf::Color(150, 150, 150, 255), sf::Color(120, 120, 120, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
	buttons.at(key)->set_render(false);
}

const bool PauseMenu::is_button_pressed(const std::string key)
{
	return buttons.at(key)->is_pressed();
}

void PauseMenu::update()
{
	for (auto& i : buttons)
	{
		i.second->set_render( get_render() );
		i.second->update();
	}
}

void PauseMenu::add_to_buffer()
{
	
	//Renderer::add(Renderer::RenderObject(&background, this));
	//Renderer::add(Renderer::RenderObject(&container, this));
	for (auto& i : buttons)
	{
		i.second->add_to_buffer();
	}
	//Renderer::add(Renderer::RenderObject(&menu_text, this));
}
void PauseMenu::close()
{
	//Time::Instance()->set_time_scale(previous_time_scale);
	set_render(false);
	for (auto& i : buttons)
		i.second->set_render(false);
}
void PauseMenu::open()
{
	//previous_time_scale = Time::Instance()->get_time_scale();
	//Time::Instance()->set_time_scale(0.f);
	set_render(true);
	for (auto& i : buttons)
		i.second->set_render(true);
}
}
