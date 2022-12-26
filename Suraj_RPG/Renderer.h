#pragma once
#include "globals.h"
#include "IRenderable.h"
#include <limits>

namespace bm98::core
{
extern float UNIT_SIZE;
extern float SCALE;

class Renderer
	: sf::RenderTarget
{
public:
	struct RenderObject
	{
		RenderObject(sf::Drawable* drawable, bool& render, SortingLayer& sorting_layer,
			unsigned char& z_order, sf::View** view = nullptr)
			:render(render), sorting_layer(sorting_layer), z_order(z_order)
		{
			this->sorting_layer = sorting_layer;
			this->drawable = drawable;
			this->z_order = z_order;
			this->view = view;
			this->private_entry_key = Renderer::get_id();
			Renderer::increase_id();
		}

		RenderObject(sf::Drawable* drawable, IRenderable* renderable)
			:render(renderable->get_render()), sorting_layer(renderable->get_sorting_layer()),
			z_order(renderable->get_z_order())
		{
			this->sorting_layer = renderable->get_sorting_layer();
			this->z_order = renderable->get_z_order();
			this->view = renderable->get_view_pointer();
			this->drawable = drawable;
			this->private_entry_key = Renderer::get_id();
			Renderer::increase_id();
		}

		SortingLayer& sorting_layer;
		bool& render;
		sf::Drawable* drawable;
		unsigned char& z_order;
		sf::View** view;
		unsigned private_entry_key;

	};

	static void init(RenderTarget* render_target, RenderTarget* dev_render_target);

	static void add(RenderObject render_object);
	static void remove(sf::Drawable* drawable);
	static void remove(RenderObject render_object);

	/// <summary>
	/// Returns true if element is top ui element under the mouse.
	/// ASSUMES ALL UI ELEMENTS USE A RECTANGLE SHAPE. ACCOUNT FOR THIS WHEN CHANGING UI
	/// IN FUTURE.
	/// </summary>
	/// <param name="drawable"></param>
	/// <param name=""></param>
	/// <returns></returns>
	static bool top_ui_under_mouse(sf::Drawable* drawable, sf::View* view = nullptr);

	/// <summary>
	/// Updates render objects after a change has been made to its sorting_layer or z_order
	/// </summary>
	static void refresh();

	static void set_view(sf::View view = window->getDefaultView());
	//static const bool& top_ui(const sf::Drawable& drawable);
	//static void add_tile(RenderObject render_object);
	// should run once on state change


	static void render();
	static void clear();

	static sf::Vector2u get_window_size();
	static sf::RenderTarget* get_dev_window();

	static const unsigned& get_id();
	static void increase_id();

protected:

private:
	static RenderTarget* window;
	static RenderTarget* dev_window;

	struct cmpStruct
	{
		bool operator() (const RenderObject ro1, const RenderObject ro2) const
		{
			if ((unsigned)ro2.sorting_layer == (unsigned)ro1.sorting_layer)
			{
				if (ro2.z_order != ro1.z_order)
					return (ro2.z_order > ro1.z_order);
				return ro2.private_entry_key > ro1.private_entry_key;
			}
			return ((unsigned)ro2.sorting_layer > (unsigned)ro1.sorting_layer);
		}
	};

	static unsigned id;
	static std::set<RenderObject, cmpStruct> render_objects;
	//static sf::Drawable* _top_ui;

};

}