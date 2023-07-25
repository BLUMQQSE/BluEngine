#include "../pch.h"
#include "../globals.h"
#include "Renderer.h"
#include "Debug.h"

namespace bm98::core
{

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
			if ((int)ui_render_object.z_order < (int)it->z_order)
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
	
	//Debug::Instance()->core_log("RENDERER ATTEMPTED TO REMOVE NON-EXISTENT RENDERABLE", Debug::LogLevel::WARNING);
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

	//Debug::Instance()->core_log("RENDERER ATTEMPTED TO REMOVE NON-EXISTENT UI RENDERABLE", Debug::LogLevel::WARNING);

}

void Renderer::draw(sf::Drawable* drawable, sf::View* view, sf::Shader* shader)
{
	window->setView(window->getDefaultView());

	if (view)
		window->setView(*view);

	if (shader)
	{
		window->draw(*drawable, shader);
		return;
	}
	window->draw(*drawable);
	window->setView(window->getDefaultView());
}

void Renderer::clear_screen()
{
	window->clear();
}

void Renderer::display()
{
	static_cast<sf::RenderWindow*>(window)->display();
}

void Renderer::refresh()
{
	//Debug::Instance()->core_log("RENDERER REFRESHING: " + std::to_string(renderables.size()) + " objects");
	std::vector<RenderObject> objects;
	for (auto& f : renderables)
		objects.push_back(f);
	renderables.clear();
	for (auto& o : objects)
		add(o);
}

void Renderer::render()
{
	if (use_y_sort)
		render_y_sorted_list(renderables);
	else
		render_list(renderables);

	if(draw_gizmos)
		EventSystem::Instance()->push_event(EventID::_SYSTEM_RENDERER_DRAW_GIZMOS_);
	
	render_list(ui_renderables);
}

void Renderer::render_list(std::list<RenderObject>& list)
{
	std::list<Renderer::RenderObject>::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (!it->render)
			continue;

		// reset view to default in case was changed in last loop
		window->setView(window->getDefaultView());

		if (it->view)
			if (*it->view)
				window->setView(**it->view);

		if (it->shaders)
		{
			for (auto& s : *it->shaders)
			{
				temp_render_with_shader(it->drawable, s.second);
			}
		}
		window->draw(*it->drawable);
	}
}

//not working
void Renderer::render_y_sorted_list(std::list<RenderObject>& list)
{
	// Only need to check to reorganize sections containing same SortingLayer && z_order
	// We can check to sort each of these individual groupings, then render that batch
	std::list<Renderer::RenderObject>::iterator it;
	std::list<Renderer::RenderObject>::iterator batch_it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		// find current batch
		Sorting::Layer layer = it->sorting_layer;
		char z = it->z_order;
		batch_it = it;
		std::vector<RenderObject> batch;
		while (batch_it->sorting_layer == layer && batch_it->z_order == z)
		{
			batch.push_back(*batch_it);
			if (std::next(batch_it) == list.end())
				break;
			else
				batch_it++;
		}


		// gotta work out how to sort this shit...
		std::vector<TestShit> test = sort_batch(batch);

		for (int i = 0; i < test.size(); i++)
		{
			if (!test[i].render)
				continue;
			// reset view to default in case was changed in last loop
			window->setView(window->getDefaultView());

			if (test[i].view)
				if (*test[i].view)
					window->setView(**test[i].view);

			if (test[i].shaders)
			{
				for (auto& s : *test[i].shaders)
				{
					temp_render_with_shader(test[i].drawable, s.second);
					// TODO: Need to apply shaders and draw final drawable

					//window->draw(*test[i].drawable, *test[i].shader);
					//continue;
				}
			}
			window->draw(*test[i].drawable);
		}
		it = batch_it;
		
		if (!batch_it->render)
			continue;
		// reset view to default in case was changed in last loop
		window->setView(window->getDefaultView());

		if (batch_it->view)
			if (*batch_it->view)
				window->setView(**batch_it->view);

		if(batch_it->shaders)
		{ 
			for (auto& s : *batch_it->shaders)
			{
				temp_render_with_shader(batch_it->drawable, s.second);
				// TODO: Need to apply shaders and draw final drawable

				//window->draw(*batch_it->drawable, *batch_it->shader);
				//continue;
			}
		}
		window->draw(*batch_it->drawable);


		continue;

		// now render batch
		for (it; it != batch_it; ++it)
		{
			if (!it->render)
				continue;
			// reset view to default in case was changed in last loop
			window->setView(window->getDefaultView());

			if (it->view)
				if (*it->view)
					window->setView(**it->view);

			if (it->shaders)
			{
				for (auto& s : *it->shaders)
				{
					temp_render_with_shader(it->drawable, s.second);
					// TODO: Need to apply shaders and draw final drawable

					//window->draw(*it->drawable, *it->shader);
					continue;
				}
			}
			window->draw(*it->drawable);
		}

		if (!batch_it->render)
			continue;
		// reset view to default in case was changed in last loop
		window->setView(window->getDefaultView());

		if (batch_it->view)
			if (*batch_it->view)
				window->setView(**batch_it->view);

		if (batch_it->shaders)
		{
			for (auto& s : *batch_it->shaders)
			{
				temp_render_with_shader(batch_it->drawable, s.second);
				// TODO: Need to apply shaders and draw final drawable

				//window->draw(*batch_it->drawable, *batch_it->shader);
				continue;
			}
		}
		window->draw(*batch_it->drawable);
		
	}
}

void Renderer::clear()
{
	renderables.clear();
	ui_renderables.clear();
}

void Renderer::fixed_update()
{

}

void Renderer::update_top_ui()
{
	if (Input::Instance()->get_mouse_up(Input::Mouse::LEFT))
	{
		top_ui = nullptr;
		return;
	}
	std::list<RenderObject>::reverse_iterator it;
	for (it = ui_renderables.rbegin(); it != ui_renderables.rend(); ++it)
	{
		if (!it->render)
			continue;

		//if (Input::Instance()->get_mouse_down(Input::Mouse::LEFT))
		//{
			if (it->has_global_bounds)
			{
				//std::cout << "found something with global bounds\n";
				sf::Shape* shape = static_cast<sf::Shape*>(it->drawable);
				if (shape->getGlobalBounds().contains(Input::Instance()->mouse_position(*it->view)))
				{
					//std::cout << "found a top ui\n";
					top_ui = it->drawable;
					return;
				}
			}
		//}
	}

}

std::vector<Renderer::TestShit> Renderer::sort_batch(std::vector<Renderer::RenderObject> batch)
{
	std::vector<Renderer::TestShit> test(batch.size());
	for(int i = 0; i < batch.size(); i++)
		test[i] = batch[i];

	for (int k = 1; k < test.size(); k++)
	{
		TestShit temp = test[k];
		int j = k - 1;
		//					move forward no matter what is lower depth		otherwise move forward if higher(lower) y_pos
		while (j >= 0 && (temp.render_depth < test[j].render_depth ||     (temp.y_pos < test[j].y_pos && temp.render_depth == test[j].render_depth)))
		{
			test[j + 1] = test[j];
			j = j - 1;
		}
		test[j + 1] = temp;
	}
	return test;
}


void Renderer::temp_render_with_shader(sf::Drawable* drawable, sf::Shader* shader)
{
	window->draw(*drawable, shader);
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
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_UPDATE_TOP_UI_, this);
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
	case EventID::_SYSTEM_RENDERER_UPDATE_TOP_UI_:
		update_top_ui();
		break;
	}
}

sf::Vector2u Renderer::getSize() const
{
	return sf::Vector2u();
	//return sf::RenderTarget::getSize();
}


}