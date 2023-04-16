#include "pch.h"
#include "globals.h"
#include "Renderer.h"
#include "Debug.h"

namespace bm98::core
{
//std::set<Renderer::RenderObject, Renderer::cmpStruct> Renderer::renderables;
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
	std::list<RenderObject>::iterator it;
	for (it = renderables.begin(); it != renderables.end(); ++it)
	{
		if (it->sorting_layer == render_object.sorting_layer)
		{
			if (render_object.z_order < it->z_order)
			{
				if (it == renderables.begin())
					renderables.push_front(render_object);
				else
					renderables.insert(it, render_object);
				return;
			}
		}
		if (render_object.sorting_layer < it->sorting_layer)
		{
			if (it == renderables.begin())
				renderables.push_front(render_object);
			else
				renderables.insert(it, render_object);
			
			return;
		}
	}
	

	// never got placed, so put in back spot
	renderables.push_back(render_object);
	
	//renderables.insert(render_object);
}

void Renderer::add_ui(RenderObject ui_render_object)
{
	std::list<RenderObject>::iterator it;
	for (it = ui_renderables.begin(); it != ui_renderables.end(); ++it)
	{
		if (it->sorting_layer == ui_render_object.sorting_layer)
		{
			if (ui_render_object.z_order < it->z_order)
			{
				if (it == ui_renderables.begin())
					ui_renderables.push_front(ui_render_object);
				else
					ui_renderables.insert(it, ui_render_object);
				return;
			}
		}
		if (ui_render_object.sorting_layer < it->sorting_layer)
		{
			if (it == ui_renderables.begin())
				ui_renderables.push_front(ui_render_object);
			else
				ui_renderables.insert(it, ui_render_object);

			return;
		}
	}


	// never got placed, so put in back spot
	ui_renderables.push_back(ui_render_object);

}

void Renderer::add_gizmo(GizmoObject gizmo)
{
	if (!draw_gizmos)
		return;
	gizmos.push_back(gizmo);
}


void Renderer::remove(sf::Drawable* drawable)
{	
	
	std::list<RenderObject>::iterator it;
	for (it = renderables.begin(); it != renderables.end(); ++it)
	{
		if (it->drawable == drawable)
		{
			renderables.erase(it);
			return;
		}
	}
	
	Debug::Instance()->core_log("RENDERER ATTEMPTED TO REMOVE NON-EXISTENT RENDERABLE", Debug::LogLevel::WARNING);

}

void Renderer::remove_ui(sf::Drawable* drawable)
{
	std::list<RenderObject>::iterator it;
	for (it = ui_renderables.begin(); it != ui_renderables.end(); ++it)
	{
		if (it->drawable == drawable)
		{
			ui_renderables.erase(it);
			return;
		}
	}

	Debug::Instance()->core_log("RENDERER ATTEMPTED TO REMOVE NON-EXISTENT UI RENDERABLE", Debug::LogLevel::WARNING);

}

//Work in progress
/*
bool Renderer::top_ui_under_mouse(sf::Drawable* drawable, sf::View* view)
{
	
	// need to find RenderObject matchign drawable
	// need to check for any elements which contains mouse position and is higher
	std::set<Renderer::RenderObject>::iterator iter;
	for (iter = renderables.begin(); iter != renderables.end(); ++iter)
	{
		if (iter->drawable == drawable)
		{
			std::set<Renderer::RenderObject>::iterator iter2;
			bool top = false;
			for (iter2 = std::next(iter, 1); iter2 != renderables.end(); ++iter2)
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
	Debug::Instance()->core_log("RENDERER REFRESHING: " + std::to_string(renderables.size()) + " objects");
	std::vector<RenderObject> objects;
	for (auto& f : renderables)
		objects.push_back(f);
	renderables.clear();
	for (auto& o : objects)
		add(o);
}

void Renderer::render()
{
	std::list<RenderObject>::iterator it;
	for (it = renderables.begin(); it != renderables.end(); ++it)
	{
		if (!it->render)
			continue;

		// reset view to default in case was changed in last loop
		window->setView(window->getDefaultView());

		if (it->view)
			if (*it->view)
				window->setView(**it->view);

		if (it->shader)
		{
			window->draw(*it->drawable, *it->shader);
			continue;
		}
		window->draw(*it->drawable);
	}

	render_gizmos();
	render_ui();
	
	
}

void Renderer::render_ui()
{
	std::list<RenderObject>::iterator it;
	for (it = ui_renderables.begin(); it != ui_renderables.end(); ++it)
	{
		if (!it->render)
			continue;

		// reset view to default in case was changed in last loop
		window->setView(window->getDefaultView());

		if (it->view)
			if (*it->view)
				window->setView(**it->view);

		if (it->shader)
		{
			window->draw(*it->drawable, *it->shader);
			continue;
		}
		window->draw(*it->drawable);
	}
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
	renderables.clear();
	ui_renderables.clear();
}

void Renderer::clear_gizmos()
{
	gizmos.clear();
}

void Renderer::fixed_update()
{
}

void Renderer::sort()
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