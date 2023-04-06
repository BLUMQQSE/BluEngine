#include "pch.h"
#include "globals.h"
#include "Renderer.h"

namespace bm98::core
{
//std::set<Renderer::RenderObject, Renderer::cmpStruct> Renderer::render_objects;
//std::vector<Renderer::GizmoObject> Renderer::gizmos;
//bool Renderer::draw_gizmos = true;
//unsigned Renderer::id = 0;
//sf::RenderTarget* Renderer::window;

float UNIT_SIZE = 32.f;
float SCALE = 2.f;

void Renderer::init(RenderTarget* render_target)
{
	window = render_target;
}

void Renderer::add(RenderObject render_object)
{
	render_objects.insert(render_object);
}

void Renderer::add_gizmo(GizmoObject gizmo)
{
	if (!draw_gizmos)
		return;
	gizmos.push_back(gizmo);
}

void Renderer::remove(sf::Drawable* drawable)
{
	for (const auto& f : render_objects)
	{
		if (f.drawable == drawable)
		{
			render_objects.erase(f);
			return;
		}
	}
}

void Renderer::remove(RenderObject render_object)
{
	render_objects.erase(render_object);
}

//Work in progress
/*
bool Renderer::top_ui_under_mouse(sf::Drawable* drawable, sf::View* view)
{
	
	// need to find RenderObject matchign drawable
	// need to check for any elements which contains mouse position and is higher
	std::set<Renderer::RenderObject>::iterator iter;
	for (iter = render_objects.begin(); iter != render_objects.end(); ++iter)
	{
		if (iter->drawable == drawable)
		{
			std::set<Renderer::RenderObject>::iterator iter2;
			bool top = false;
			for (iter2 = std::next(iter, 1); iter2 != render_objects.end(); ++iter2)
			{
				if (dynamic_cast<sf::RectangleShape*>(iter2->drawable))
				{
					// this is issue, because with drop down will always register on top of other elements event when closed
					if(dynamic_cast<sf::RectangleShape*>(iter2->drawable)->getGlobalBounds().contains(Input::mouse_position(view)) && iter2->render)
						return false;
				}
			}
			return true;
		}
	}
	return true;
}
*/

void Renderer::refresh()
{
	std::vector<RenderObject> objects;
	for (auto& f : render_objects)
		objects.push_back(f);
	render_objects.clear();
	for (auto& o : objects)
		add(o);
}

void Renderer::set_view(sf::View view)
{
	window->setView(view);
}

void Renderer::render()
{
	
	for (const auto& f : render_objects)
	{	
		if (!f.render)
			continue;

		if (f.view)
			if(*f.view)
				window->setView(**f.view);
			else
				window->setView(window->getDefaultView());
		
		if(f.shader)
		{
			window->draw(*f.drawable, *f.shader);
			continue;
		}
		window->draw(*f.drawable);
	}
	render_gizmos();
}

void Renderer::render_gizmos()
{
	for (const auto& g : gizmos)
	{
		window->draw(g.drawable);
	}
}

void Renderer::clear()
{
	render_objects.clear();
	id = 0;
}

void Renderer::clear_gizmos()
{
	gizmos.clear();
}

void Renderer::fixed_update()
{
}

sf::Vector2u Renderer::get_window_size()
{
	return window->getSize();
}

const sf::RenderTarget* Renderer::get_window() const
{
	return window;
}

Renderer::Renderer()
{
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_INITIALIZE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_REFRESH_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_RENDER_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_CLEAR_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_FIXED_UPDATE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_CLEAR_GIZMOS_, this);
}

const unsigned& Renderer::get_id()
{
	return id;
}

void Renderer::increase_id()
{
	id++;
}

void Renderer::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
	case EventID::_SYSTEM_RENDERER_INITIALIZE_:
	{
		RenderTarget* tar = static_cast<RenderTarget*>(event->get_parameter());
		init(tar);
		break;	
	}
	case EventID::_SYSTEM_RENDERER_RENDER_:
		render();
		break;
	case EventID::_SYSTEM_RENDERER_CLEAR_:
		clear();
		break;
	case EventID::_SYSTEM_RENDERER_FIXED_UPDATE_:
		fixed_update();
		break;
	case EventID::_SYSTEM_RENDERER_REFRESH_:
		refresh();
		break;
	case EventID::_SYSTEM_RENDERER_CLEAR_GIZMOS_:
		clear_gizmos();
		break;
	}
}

sf::Vector2u Renderer::getSize() const
{
	return sf::Vector2u();
	//return sf::RenderTarget::getSize();
}


}