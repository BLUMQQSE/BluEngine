#include "pch.h"
#include "IRenderable.h"
#include "Renderer.h"

namespace bm98
{

void IRenderable::set_view(sf::View* view)
{
	this->view = view;
}
void IRenderable::set_render(bool render)
{
	this->render = render;
}
void IRenderable::set_z_order(unsigned char order, bool refresh_renderer)
{
	this->z_order = order;
	if(refresh_renderer)
		core::Renderer::refresh();
}
void IRenderable::set_sorting_layer(SortingLayer layer, bool refresh_renderer)
{
	this->sorting_layer = layer;
	if (refresh_renderer)
		core::Renderer::refresh();
	
}
bool& IRenderable::get_render()
{
	return render;
}
SortingLayer& IRenderable::get_sorting_layer()
{
	return sorting_layer;
}
unsigned char& IRenderable::get_z_order()
{
	return z_order;
}
sf::View** IRenderable::get_view_pointer()
{
	return &view;
}
sf::View* IRenderable::get_view()
{
	return view;
}

Json::Value bm98::IRenderable::serialize_json()
{
	Json::Value obj;

	obj["sorting-layer"] = Global::layer_to_string(sorting_layer);
	obj["z-order"] = z_order;
	obj["render"] = render;

	return obj;
}

void bm98::IRenderable::unserialize_json(Json::Value obj)
{
	sorting_layer = Global::string_to_layer(obj["sorting-layer"].asString());
	z_order = static_cast<unsigned char>(obj["z-order"].asInt64());
	render = obj["render"].asBool();
}
}