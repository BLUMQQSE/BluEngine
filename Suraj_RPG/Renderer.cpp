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

void Renderer::set_view(sf::View view)
{
	window->setView(view);
}

void Renderer::render()
{
	for (const auto& f : render_objects)
	{
		/* currently throws null errors... work in progress
		if (f.alternate_target)
		{
			f.alternate_target->draw(*f.drawable);
		}
		else
		*/
		
		if (!f.render)
			continue;

		if (f.view)
			if(*f.view)
				window->setView(**f.view);
			else
				window->setView(window->getDefaultView());
			
		window->draw(*f.drawable);

	}
}

void Renderer::clear()
{
	/*
	std::set<RenderObject, cmpStruct>::reverse_iterator rit;

	for (rit = render_objects.rbegin(); rit != render_objects.rend(); rit++)
	{
		if (rit->sorting_layer != SortingLayer::UI)
			break;
		if(rit->drawable)
	}
	*/
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