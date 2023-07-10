#pragma once
#include "globals.h"
#include "IData.h"
namespace bm98
{
class IRenderable : public IData
{
public:

	IRenderable();
	IRenderable(Sorting::Layer layer, char z_order, bool render, 
				sf::View* view = nullptr, sf::Shader* shader = nullptr);
	virtual ~IRenderable();

	virtual void set_view(sf::View* view);
	
	
	//void set_shader(std::pair<std::string, std::string> shader_files, sf::Vector2f light_pos);


	void set_shader(sf::Shader* shader);
	//void set_uniforms(sf::Vector2f light_pos);
	virtual void set_render(bool render);
	void set_z_order(char order, bool refresh_renderer = true);
	void set_sorting_layer(Sorting::Layer layer, bool refresh_renderer = true);
	void set_render_depth(int depth);
	void set_y_pos(float y_pos);

	bool& get_render();
	Sorting::Layer& get_sorting_layer();
	char& get_z_order();
	sf::View** get_view_pointer();
	sf::View* get_view();
	sf::Shader** get_shader_pointer();
	sf::Shader* get_shader();
	float& get_y_pos();
	int& get_render_depth();

	std::map<std::string, sf::Shader*>* get_shaders() { return &shaders; };
	std::map<std::string, std::vector<std::tuple<std::string, void*, Var::Type>>>* get_uniforms() 
	{ return &uniforms; }

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

private:
	Sorting::Layer sorting_layer = Sorting::Layer::BACKGROUND;
	char z_order = 0;
	// Used by SpriteComponents with sorting_group
	float y_pos;
	int render_depth = 0;
	sf::View* view = nullptr;
	sf::Shader* shader = nullptr;
	
	std::map<std::string, sf::Shader*> shaders;
	std::map<std::string, std::vector<std::tuple<std::string, void*, Var::Type>>> uniforms;

	bool render = true;

};

}