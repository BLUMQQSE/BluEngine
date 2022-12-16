#include "pch.h"
#include "PauseMenu.h"
#include "Renderer.h"
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


	this->render = false;
	this->sorting_layer = SortingLayer::UI;
	Renderer::add(Renderer::RenderObject(&background, this));
	Renderer::add(Renderer::RenderObject(&container, this));
	Renderer::add(Renderer::RenderObject(&menu_text, this));
}

PauseMenu::~PauseMenu()
{
	Renderer::remove(&background);
	Renderer::remove(&container);
	Renderer::remove(&menu_text);
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != buttons.end(); ++it)
	{
		delete it->second;
	}
}

std::map<std::string, GUI::Button*>& PauseMenu::get_buttons()
{
	return buttons;
}

void PauseMenu::add_button(const std::string key, float x, float y, const std::string text)
{
	buttons[key] = new GUI::Button(x, y, 150.f, 60.f, &font, text, 28,
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
		i.second->set_render(this->render);
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
	render = false;
	for (auto& i : buttons)
		i.second->set_render(false);
}
void PauseMenu::open()
{
	render = true;
	for (auto& i : buttons)
		i.second->set_render(true);
}
}
