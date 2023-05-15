#include "../pch.h"

#include "UITagSystem.h"
#include "Debug.h"
#include "ResourceManager.h"
#include "Time.h"
#include "SceneManager.h"

#include "../GameObject.h"

namespace bm98
{
UITag::UITag()
{
	// Default the tag to only do during_action 
	//final_action.start_time = INFINITY;
	actions.push_back(ActionSegment(Action::NO_ACTION, 0, 0));
}
void UITag::update()
{	
	if (actions.size() > 1)
	{
		std::list<ActionSegment>::iterator it = std::next(actions.begin());
		if (timer.get_elapsed_time().asSeconds() > it->start_time)
			actions.pop_front();
	}

	switch (actions.front().action)
	{
		case Action::NO_ACTION:
			no_action(actions.front());
			break;
		case Action::DISAPPEAR:
			disappear(actions.front());
			break;
		case Action::MOVE_AND_FADE:
			move_and_fade(actions.front(),
						  static_cast<std::pair<Vector2f, float>*>(actions.front().value)->first,
						  static_cast<std::pair<Vector2f, float>*>(actions.front().value)->second);
			break;
		case Action::FOLLOW:
			follow(actions.front());
			break;
		case Action::EXPLODE:
			explode(actions.front(), *static_cast<float*>(actions.front().value));
			break;
		case Action::EXPLODE_AND_FADE:
			explode_and_fade(actions.front(),
						  static_cast<std::pair<float, float>*>(actions.front().value)->first,
						  static_cast<std::pair<float, float>*>(actions.front().value)->second);
			break;
		case Action::MOVE:
			move(actions.front(), *static_cast<Vector2f*>(actions.front().value));
			break;
		case Action::FADE:
			fade(actions.front(), *static_cast<float*>(actions.front().value));
			break;
		default:
			break;
	}
}

void UITag::add_action(Action action, float start_time, void* value = nullptr)
{
	actions.push_back(ActionSegment(action, start_time, value));
}

#pragma region TEXT_UI

TextUITag::TextUITag(Vector2f position, std::string text, unsigned font_size, float duration,
					 sf::View* view)
{
	int x = (int)position.x;
	int y = (int)position.y;
	rt = new GUI::RichText(position, text, font_size, 10000, sf::Color::White, sf::Color::Black);
	this->fill_color = sf::Color::White;
	this->outline_color = sf::Color::Black;

	this->duration = duration;

	rt->set_view(view);
	rt->set_render(false);
}

TextUITag::~TextUITag()
{
	delete rt;
}

void TextUITag::set_view(sf::View* view)
{
	rt->set_view(view);
}

void TextUITag::no_action(const ActionSegment& action)
{
	rt->set_render(true);
	rt->set_fill_color(fill_color);
	rt->set_outline_color(outline_color);
}

void TextUITag::disappear(const ActionSegment& action)
{
	rt->set_render(false);
}

void TextUITag::move(const ActionSegment& action, Vector2f move_speed)
{
	rt->set_render(true);
	rt->move(Vector2f(move_speed.x * 40 * core::Time::Instance()->delta_time(), move_speed.y * 40 * core::Time::Instance()->delta_time()));
}

bool TextUITag::fade(const ActionSegment& action, float fade_speed)
{
	rt->set_render(true);
	sf::Color c = rt->get_fill_color();
	sf::Color cc = rt->get_outline_color();

	if (c.a == 0 && cc.a == 0)
		return false;

	rt->set_fill_color(sf::Color(c.r, c.g, c.b, c.a - fade_speed * core::Time::Instance()->delta_time()));
	rt->set_outline_color(sf::Color(cc.r, cc.g, cc.b, cc.a - fade_speed * core::Time::Instance()->delta_time()));
	return true;
}

void TextUITag::follow(const ActionSegment& action)
{
	rt->set_render(true);
	rt->set_fill_color(fill_color);
	rt->set_outline_color(outline_color);
	
	Vector2f pos = *static_cast<Vector2f*>(action.value);
	rt->set_position(pos.x, pos.y);
}

void TextUITag::explode(const ActionSegment& action, float explode_speed)
{
	rt->set_render(true);
	
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

	//core::UITagSystem::Instance()->add(this);

	set_view(view);
	set_render(false);
	core::Renderer::Instance()->add_ui(core::Renderer::RenderObject(&this->image, this));
}

ImageUITag::~ImageUITag()
{
	core::Renderer::Instance()->remove_ui(&this->image);
}

void ImageUITag::no_action(const ActionSegment& action)
{
	image.setFillColor(sf::Color::White);
	set_render(true);
}

void ImageUITag::disappear(const ActionSegment& action)
{
	set_render(false);
}

void ImageUITag::move(const ActionSegment& action, Vector2f move_speed)
{
	set_render(true);
	image.move(move_speed.x * 40 * core::Time::Instance()->delta_time(), move_speed.y * 40 * core::Time::Instance()->delta_time());
}

bool ImageUITag::fade(const ActionSegment& action, float fade_speed)
{
	set_render(true);
	sf::Color c = image.getFillColor();
	sf::Color cc = image.getOutlineColor();

	if (c.a == 0 && cc.a == 0)
		return false;

	image.setFillColor(sf::Color(c.r, c.g, c.b, c.a - fade_speed * core::Time::Instance()->delta_time()));

	image.setOutlineColor(sf::Color(cc.r, cc.g, cc.b, cc.a - fade_speed * core::Time::Instance()->delta_time()));
	return true;
}

void ImageUITag::follow(const ActionSegment& action)
{
	set_render(true);

	Vector2f pos = *static_cast<Vector2f*>(action.value);

	image.set_position(pos);
}

void ImageUITag::explode(const ActionSegment& action, float explode_speed)
{
	set_render(true);
}

#pragma endregion


namespace core
{
void UITagSystem::update()
{
	std::list<UITag*>::iterator i = tags.begin();

	while (i != tags.end())
	{
		if((*i)->get_elapsed_time() > (*i)->get_duration())
		{
			UITag* to_delete = (*i);
			i = tags.erase(i);
			destroy(to_delete);
		}
		else
		{
			(*i)->update();
			++i;
		}
	}
}

TextUITag* UITagSystem::create_text_tag(Vector2f position, std::string text, unsigned font_size, float duration, sf::View* view)
{
	TextUITag* t = new TextUITag(position, text, font_size, duration, view);
	add(t);
	return t;
}

ImageUITag* UITagSystem::create_image_tag(Vector2f position, Vector2f size, sf::Texture* texture, float duration, sf::View* view)
{
	ImageUITag* i = new ImageUITag(position, size, texture, duration, view);
	add(i);
	return i;
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
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_SCENEMANAGER_PRE_SCENE_CHANGE_FLAG_, this);
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
		case EventID::_SYSTEM_SCENEMANAGER_PRE_SCENE_CHANGE_FLAG_:	
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