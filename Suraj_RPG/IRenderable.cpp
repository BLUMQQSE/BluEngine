#include "pch.h"
#include "core/Debug.h"
#include "IRenderable.h"
#include "core/Renderer.h"

namespace bm98
{
IRenderable::IRenderable()
{
	shader = new sf::Shader();
}
IRenderable::IRenderable(Sorting::Layer layer, char z_order, bool render,
						 sf::View* view, sf::Shader* shader)
{
	set_sorting_layer(layer);
	set_z_order(z_order);
	set_render(render);
	set_view(view);
	set_shader(shader);
}
IRenderable::~IRenderable()
{
	//if(shader)
	//	delete shader;
}
void IRenderable::set_view(sf::View* view)
{
	this->view = view;
}
/*
void IRenderable::set_shader(std::pair<std::string, std::string> shader_files, sf::Vector2f light_pos)
{
	this->shader->loadFromFile(shader_files.first, shader_files.second);
	
	this->shader->setUniform("hasTexture", true);
	this->shader->setUniform("lightPos", light_pos);


}
*/
void IRenderable::set_shader(sf::Shader* shader)
{
	this->shader = shader;
}
/*
void IRenderable::set_uniforms(sf::Vector2f light_pos)
{
	shader->setUniform("lightPos", light_pos);
}
*/
void IRenderable::set_render(bool render)
{
	this->render = render;
}

void IRenderable::set_z_order(char order, bool refresh_renderer)
{
	this->z_order = order;
	if (refresh_renderer)
	{
		core::Renderer::Instance()->refresh();
	}
}
void IRenderable::set_sorting_layer(Sorting::Layer layer, bool refresh_renderer)
{
	this->sorting_layer = layer;
	if (refresh_renderer)
	{
		core::Renderer::Instance()->refresh();
	}
}
void IRenderable::set_render_depth(int depth)
{
	this->render_depth = depth;
}
void IRenderable::set_y_pos(float y_pos)
{
	this->y_pos = y_pos;
}
bool& IRenderable::get_render()
{
	return render;
}
Sorting::Layer& IRenderable::get_sorting_layer()
{
	return sorting_layer;
}
char& IRenderable::get_z_order()
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

sf::Shader** IRenderable::get_shader_pointer()
{
	return &shader;
}

sf::Shader* IRenderable::get_shader()
{
	return shader;
}

float& IRenderable::get_y_pos()
{
	return y_pos;
}

int& IRenderable::get_render_depth()
{
	return render_depth;
}

Json::Value bm98::IRenderable::serialize_json()
{
	Json::Value obj;

	obj["sorting-layer"] = Sorting::ToString(sorting_layer);
	obj["z-order"] = z_order;
	obj["render"] = render;

	return obj;
}

void bm98::IRenderable::unserialize_json(Json::Value obj)
{
	sorting_layer = Sorting::ToLayer(obj["sorting-layer"].asString());
	z_order = static_cast<unsigned char>(obj["z-order"].asInt64());
	render = obj["render"].asBool();
}

}