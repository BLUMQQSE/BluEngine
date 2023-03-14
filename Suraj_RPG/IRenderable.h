#pragma once
#include "globals.h"
#include "IData.h"
namespace bm98
{
class IRenderable : public IData
{
public:

	IRenderable();
	virtual ~IRenderable();

	void set_view(sf::View* view);
	void set_shader(std::pair<std::string, std::string> shader_files, sf::Vector2f light_pos);
	void set_uniforms(sf::Vector2f light_pos);
	virtual void set_render(bool render);
	void set_z_order(char order, bool refresh_renderer = true);
	void set_sorting_layer(SortingLayer layer, bool refresh_renderer = true);
	
	bool& get_render();
	SortingLayer& get_sorting_layer();
	char& get_z_order();
	sf::View** get_view_pointer();
	sf::View* get_view();
	sf::Shader** get_shader_pointer();
	sf::Shader* get_shader();

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

private:
	SortingLayer sorting_layer = SortingLayer::BACKGROUND;
	char z_order = 0;
	sf::View* view = nullptr;
	sf::Shader* shader = nullptr;
	bool render = true;

};

}