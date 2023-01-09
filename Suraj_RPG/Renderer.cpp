#include "pch.h"
#include "globals.h"
#include "Renderer.h"
#include "Debug.h"

namespace bm98::core
{
std::set<Renderer::RenderObject, Renderer::cmpStruct> Renderer::render_objects;
unsigned Renderer::id = 0;
sf::RenderTarget* Renderer::window;

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
	
}

void Renderer::clear()
{
	render_objects.clear();
	id = 0;
}

sf::Vector2u Renderer::get_window_size()
{
	return window->getSize();
}

const unsigned& Renderer::get_id()
{
	return id;
}

void Renderer::increase_id()
{
	id++;
}


}