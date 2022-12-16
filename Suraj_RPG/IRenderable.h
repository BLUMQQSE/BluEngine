#pragma once
#include "globals.h"
namespace bm98
{
class IRenderable
{
public:
	void set_view(sf::View* view)
	{
		this->view = view;
	}
	void set_render(bool render)
	{
		this->render = render;
	}
	bool& get_render()
	{
		return render;
	}
	SortingLayer& get_sorting_layer()
	{
		return sorting_layer;
	}
	unsigned char& get_z_order()
	{
		return z_order;
	}
	sf::View** get_view()
	{
		return &view;
	}

protected:
	SortingLayer sorting_layer = SortingLayer::BACKGROUND;
	unsigned char z_order = 0;
	sf::View* view = nullptr;
	bool render = true;
};

}