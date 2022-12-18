#pragma once
#include "globals.h"
#include "IData.h"
namespace bm98
{
class IRenderable : public IData
{
public:

	void set_view(sf::View* view);
	void set_render(bool render);
	void set_z_order(unsigned char order, bool refresh_renderer = true);
	void set_sorting_layer(SortingLayer layer, bool refresh_renderer = true);
	
	bool& get_render();
	SortingLayer& get_sorting_layer();
	unsigned char& get_z_order();
	sf::View** get_view_pointer();
	sf::View* get_view();

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

private:
	SortingLayer sorting_layer = SortingLayer::BACKGROUND;
	unsigned char z_order = 0;
	sf::View* view = nullptr;
	bool render = true;

};

}