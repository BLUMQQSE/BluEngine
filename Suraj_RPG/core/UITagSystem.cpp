#include "../pch.h"

#include "UITagSystem.h"
#include "Debug.h"
#include "ResourceManager.h"
#include "Time.h"
#include "SceneManager.h"

namespace bm98
{
#pragma region TEXT_UI

TextUITag::TextUITag(Vector2f position, std::string text, unsigned font_size, float duration,
					 sf::View* view)
{

	int x = (int)position.x;
	int y = (int)position.y;

	this->text.setStyle(sf::Text::Bold);
	this->text.setFont(core::ResourceManager::Instance()->get_font("calibri-regular.ttf"));
	this->text.setCharacterSize(font_size);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::White);
	this->text.setOutlineThickness(1.f);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setPosition(x, y);

	this->duration = duration;
	this->movement_speed = 5;

	core::UITagSystem::Instance()->add(this);
	
	set_view(view);
	core::Renderer::Instance()->add_ui(core::Renderer::RenderObject(&this->text, this));
}

TextUITag::~TextUITag()
{
	core::Renderer::Instance()->remove_ui(&this->text);
}

bool TextUITag::move_and_fade()
{
	if (text.getFillColor().a == 0)
	{
		return false;
	}
	text.setPosition(text.getPosition().x, text.getPosition().y - movement_speed * 40 * core::Time::Instance()->delta_time());
	sf::Color c = text.getFillColor();
	text.setFillColor(sf::Color(c.r, c.g, c.b, c.a - core::Time::Instance()->delta_time()));

	return true;
}

#pragma endregion

#pragma region IMAGE_UI

ImageUITag::ImageUITag(Vector2f position, Vector2f size, sf::Texture* texture, float duration,
					   sf::View* view)
{
	image = FloatConvex::Rectangle(position, size);
	image.setTexture(texture);
	image.setFillColor(sf::Color::White);
	
	this->duration = duration;
	this->movement_speed = 5;

	core::UITagSystem::Instance()->add(this);

	set_view(view);
	core::Renderer::Instance()->add_ui(core::Renderer::RenderObject(&this->image, this));
}

ImageUITag::~ImageUITag()
{
	core::Renderer::Instance()->remove_ui(&this->image);
}

bool ImageUITag::move_and_fade()
{
	if (image.getFillColor().a == 0)
	{
		return false;
	}
	image.move(0, -movement_speed * 40 * core::Time::Instance()->delta_time());
	sf::Color c = image.getFillColor();
	image.setFillColor(sf::Color(c.r, c.g, c.b, c.a - core::Time::Instance()->delta_time()));

	return true;
}

#pragma endregion


namespace core
{
void UITagSystem::update()
{
	std::list<UITag*>::iterator i = tags.begin();

	while (i != tags.end())
	{
		if ((*i)->get_elapsed_time() >= (*i)->get_duration())
		{
			if (!(*i)->move_and_fade())
			{
				UITag* to_delete = (*i);
				i = tags.erase(i);
				destroy(to_delete);
			}
			else
				++i;
		}
		else
			++i;
	}
}

void UITagSystem::add(UITag* tag)
{
	if (std::find(tags.begin(), tags.end(), tag) == tags.end())
	{
		tag->set_render(true);
		tag->set_sorting_layer(layer);
		tag->set_z_order(z);
		tag->start_timer();

		tags.push_back(tag);
	}
}

void UITagSystem::destroy(UITag* tag)
{
	delete tag;
}

UITagSystem::UITagSystem()
{
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_UITAG_UPDATE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_SCENEMANAGER_PRE_SCENE_CHANGE_, this);
}

void UITagSystem::clear()
{
	std::list<UITag*>::iterator i = tags.begin();

	while (i != tags.end())
	{
		UITag* to_delete = (*i);
		i = tags.erase(i);
		destroy(to_delete);
	}
}

void UITagSystem::shutdown()
{
	std::list<UITag*>::iterator i = tags.begin();

	while (i != tags.end())
	{
		UITag* to_delete = (*i);
		i = tags.erase(i);
		destroy(to_delete);
	}
}

void UITagSystem::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
		case EventID::_SYSTEM_SCENEMANAGER_PRE_SCENE_CHANGE_:	
			clear();
			break;
		case EventID::_SYSTEM_UITAG_UPDATE_:
			update();
			break;
		default:
			break;
	}
}

}


}