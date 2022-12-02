#pragma once
#include "globals.h"
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
		RenderObject(sf::Drawable* drawable, SortingLayer sorting_layer,
			unsigned char z_order = 0, sf::View* view = nullptr)
		{
			this->sorting_layer = sorting_layer;
			this->drawable = drawable;
			this->z_order = z_order;
			this->view = view;
			this->private_entry_key = Renderer::get_id();
			Renderer::increase_id();
		}

		SortingLayer sorting_layer;
		sf::Drawable* drawable;
		unsigned z_order;
		sf::View* view;
		unsigned private_entry_key;

	};

	static void init(RenderTarget* render_target);

	static void add(RenderObject render_object);
	static void remove(RenderObject render_object);

	static void set_view(sf::View view = window->getDefaultView());
	//static const bool& top_ui(const sf::Drawable& drawable);
	//static void add_tile(RenderObject render_object);
	// should run once on state change


	static void render();
	static void clear();

	static sf::Vector2u get_window_size();

	static const unsigned& get_id();
	static void increase_id();

protected:

private:
	static RenderTarget* window;

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