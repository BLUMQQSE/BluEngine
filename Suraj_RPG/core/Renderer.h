#pragma once
#include "../globals.h"
#include "../IRenderable.h"
#include "Math.h"
#include "ResourceManager.h"
#include "EventSystem.h"

namespace bm98::core
{
extern float UNIT_SIZE;
extern float SCALE;

class Renderer
	: sf::RenderTarget, public Listener
{

public:
	// Returns instance of the EventSystem
	static Renderer* Instance()
	{
		static Renderer instance;
		return &instance;
	}

	struct RenderObject
	{
		// Constructor should only be used by GUI
		RenderObject(sf::Drawable* drawable, bool& render, Sorting::Layer& sorting_layer,
			char& z_order, sf::View** view = nullptr, std::map<std::string, sf::Shader*>* shaders = nullptr,
					std::map<std::string, std::pair<std::string, float>>* float_uniforms = nullptr,
					std::map<std::string, std::pair<std::string, bool>>* bool_uniforms = nullptr,
					std::map<std::string, std::pair<std::string, sf::Glsl::Vec2>>* vec2_uniforms = nullptr,
					std::map<std::string, std::pair<std::string, sf::Glsl::Vec4>>* color_uniforms = nullptr)
			:render(render), sorting_layer(sorting_layer), z_order(z_order) 
		{
			this->sorting_layer = sorting_layer;
			this->drawable = drawable;
			this->z_order = z_order;
			this->view = view;
			this->shaders = shaders;
			this->float_uniforms = float_uniforms;
			this->bool_uniforms = bool_uniforms;
			this->vec2_uniforms = vec2_uniforms;
			this->color_uniforms = color_uniforms;
		}
		// Optional extension for Y-Sort
		RenderObject(sf::Drawable* drawable, bool& render, Sorting::Layer& sorting_layer,
					char& z_order, float* y_pos, int* render_depth, sf::View** view = nullptr, 
					std::map<std::string, sf::Shader*>* shaders = nullptr,
					std::map<std::string, std::pair<std::string, float>>* float_uniforms = nullptr,
					std::map<std::string, std::pair<std::string, bool>>* bool_uniforms = nullptr,
					std::map<std::string, std::pair<std::string, sf::Glsl::Vec2>>* vec2_uniforms = nullptr,
					std::map<std::string, std::pair<std::string, sf::Glsl::Vec4>>* color_uniforms = nullptr)
			:render(render), sorting_layer(sorting_layer), z_order(z_order),
			y_pos(y_pos), render_depth(render_depth)
		{
			this->sorting_layer = sorting_layer;
			this->drawable = drawable;
			this->z_order = z_order;
			this->view = view;
			this->shaders = shaders;
			this->float_uniforms = float_uniforms;
			this->bool_uniforms = bool_uniforms;
			this->vec2_uniforms = vec2_uniforms;
			this->color_uniforms = color_uniforms;
			this->render_depth = render_depth;
			this->y_pos = y_pos;
		}
		
		// Constructor should be used by any game entity to ensure render_depth is locatable
		RenderObject(sf::Drawable* drawable, IRenderable* renderable)
			:render(renderable->get_render()), sorting_layer(renderable->get_sorting_layer()),
			z_order(renderable->get_z_order()), y_pos(&renderable->get_y_pos()),
			render_depth(&renderable->get_render_depth())
		{
			this->sorting_layer = renderable->get_sorting_layer();
			this->z_order = renderable->get_z_order();
			this->view = renderable->get_view_pointer();
			this->shaders = renderable->get_shaders();
			this->float_uniforms = renderable->get_float_uniforms();
			this->bool_uniforms = renderable->get_bool_uniforms();
			this->vec2_uniforms = renderable->get_vec2_uniforms();
			this->color_uniforms = renderable->get_color_uniforms();
			this->drawable = drawable;
		}
		
		bool operator<(const RenderObject& rhs)
		{
			if (sorting_layer == rhs.sorting_layer)
			{
				if ((int)z_order == (int)rhs.z_order)
				{
					if (*y_pos == *rhs.y_pos)
					{
						return render_depth < rhs.render_depth;
					}
					return *y_pos < *rhs.y_pos;
				}
				return (int)z_order < (int)rhs.z_order;

			}
			return sorting_layer < rhs.sorting_layer;
		}
		bool operator<=(const RenderObject& rhs)
		{
			if (sorting_layer == rhs.sorting_layer)
			{
				if ((int)z_order == (int)rhs.z_order)
				{
					if (*y_pos == *y_pos)
					{
						return render_depth <= rhs.render_depth;
					}
					return *y_pos <= *rhs.y_pos;
				}
				return (int)z_order <= (int)rhs.z_order;

			}
			return sorting_layer <= rhs.sorting_layer;
		}

		Sorting::Layer& sorting_layer;
		bool& render;
		sf::Drawable* drawable;
		char& z_order;
		
		sf::View** view;
		//sf::Shader** shader;
		std::map<std::string, sf::Shader*>* shaders = nullptr;
		std::map<std::string, std::pair<std::string, float>>* float_uniforms = nullptr;
		std::map<std::string, std::pair<std::string, bool>>* bool_uniforms = nullptr;
		std::map<std::string, std::pair<std::string, sf::Glsl::Vec2>>* vec2_uniforms = nullptr;
		std::map<std::string, std::pair<std::string, sf::Glsl::Vec4>>* color_uniforms = nullptr;


		// Used for y-sort
		float* y_pos = nullptr;
		int* render_depth = nullptr;

		// Will be used by ui elements
		bool has_global_bounds = false;

	};

	struct TestShit
	{
		TestShit() {}
		TestShit(Sorting::Layer layer, bool render, sf::Drawable* drawable, char z_order,
				sf::View** view, std::map<std::string, sf::Shader*>* shaders,
				std::map<std::string, std::pair<std::string, float>>* float_uniforms,
				std::map<std::string, std::pair<std::string, bool>>* bool_uniforms,
				std::map<std::string, std::pair<std::string, sf::Glsl::Vec2>>* vec2_uniforms,
				std::map<std::string, std::pair<std::string, sf::Glsl::Vec4>>* color_uniforms,
				 float y_pos, int render_depth, bool has_global_bounds)
			:sorting_layer(layer), render(render), drawable(drawable), z_order(z_order),
			view(view), shaders(shaders), float_uniforms(float_uniforms), bool_uniforms(bool_uniforms), vec2_uniforms(vec2_uniforms),
			color_uniforms(color_uniforms), y_pos(y_pos), render_depth(render_depth),
			has_global_bounds(has_global_bounds)
		{

		}
		TestShit(const RenderObject& r)
		{
			sorting_layer = r.sorting_layer;
			render = r.render;
			drawable = r.drawable;
			z_order = r.z_order;
			view = r.view;
			shaders = r.shaders;
			float_uniforms = r.float_uniforms;
			bool_uniforms = r.bool_uniforms;
			vec2_uniforms = r.vec2_uniforms;
			color_uniforms = r.color_uniforms;
			y_pos = *r.y_pos;
			render_depth = *r.render_depth;
			has_global_bounds = r.has_global_bounds;
		}

		Sorting::Layer sorting_layer;
		bool render;
		sf::Drawable* drawable = nullptr;
		char z_order;

		sf::View** view = nullptr;

		std::map<std::string, sf::Shader*>* shaders = nullptr;
		std::map<std::string, std::pair<std::string, float>>* float_uniforms = nullptr;
		std::map<std::string, std::pair<std::string, bool>>* bool_uniforms = nullptr;
		std::map<std::string, std::pair<std::string, sf::Glsl::Vec2>>* vec2_uniforms = nullptr;
		std::map<std::string, std::pair<std::string, sf::Glsl::Vec4>>* color_uniforms = nullptr;


		// Used for y-sort
		float y_pos;
		int render_depth;

		// Will be used by ui elements
		bool has_global_bounds = false;
	};

	inline void set_draw_gizmos(bool draw) { draw_gizmos = draw; }
	inline const bool get_draw_gizmos() { return draw_gizmos; }

	inline const unsigned get_renderables_count() { return renderables.size(); }
	inline const unsigned get_ui_renderables_count() { return ui_renderables.size(); }

	void init(RenderTarget* render_target);

	inline bool is_top_ui(sf::Drawable* drawable) { return drawable == top_ui; }

	void add(RenderObject render_object);
	void add_ui(RenderObject ui_render_object);

	void remove(sf::Drawable* drawable);
	void remove_ui(sf::Drawable* drawable);

	/// <summary>
	/// Immediate draw call to the window. Will be used for Loading Screens when running program
	/// temporarily on seperate thread.
	/// </summary>
	void draw(sf::Drawable* drawable, sf::View* view = nullptr,
		sf::Shader* shader = nullptr);
	/// <summary>
	/// Clears the screen.
	/// </summary>
	void clear_screen();

	void display();

	/// <summary>
	/// Returns true if element is top ui element under the mouse.
	/// ASSUMES ALL UI ELEMENTS USE A RECTANGLE SHAPE. ACCOUNT FOR THIS WHEN CHANGING UI
	/// IN FUTURE.
	/// </summary>
	/// <param name="drawable"></param>
	/// <param name=""></param>
	/// <returns></returns>
	//static bool top_ui_under_mouse(sf::Drawable* drawable, sf::View* view = nullptr);

	/// <summary>
	/// Updates render objects after a change has been made to its sorting_layer or z_order
	/// </summary>
	void refresh();

	//static const bool& top_ui(const sf::Drawable& drawable);
	//static void add_tile(RenderObject render_object);
	// should run once on state change

	sf::Vector2u get_window_size();
	const sf::RenderTarget* get_window() const;

protected:

private:

	Renderer();
	~Renderer() { }
	Renderer(const Renderer& rhs){}
	Renderer& operator=(const Renderer& rhs) {}

	RenderTarget* window;

	std::list<RenderObject> renderables;
	std::list<RenderObject> ui_renderables;

	sf::Drawable* top_ui;


	// Inherited via RenderTarget
	virtual sf::Vector2u getSize() const override;

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

	void render();
	void render_list(std::list<RenderObject>& list);
	void render_y_sorted_list(std::list<RenderObject>& list);

	void clear();
	void fixed_update();

	void update_top_ui();

	void temp_render_with_shader(sf::Drawable* drawable, sf::Shader* shaders);

	std::vector<TestShit> sort_batch(std::vector<RenderObject> batch);

	bool draw_gizmos = false;
	bool use_y_sort = true;
};


class Gizmo
{
public:

	static sf::Color outline_color; //= sf::Color::White;
	static sf::Color fill_color; //= sf::Color::Transparent;
	static float outline_thickness; //= 1;

	static void set_view(sf::View* v) { view = v; }

	static void reset_values()
	{
		outline_color = sf::Color::White;
		fill_color = sf::Color::Transparent;
		outline_thickness = 1;
	}

	static void draw_vertex_array(sf::VertexArray& v, sf::Shader* shader = nullptr)
	{
		Renderer::Instance()->draw(&v, view, shader);
	}

	static void draw_convex(FloatConvex& convex)
	{
		convex.setFillColor(fill_color);
		convex.setOutlineColor(outline_color);
		convex.setOutlineThickness(outline_thickness);

		Renderer::Instance()->draw(&convex, view);
	}

	static void draw_line(Vector2f from, Vector2f to)
	{
		FloatConvex line = FloatConvex::Line(from, to, 1.f);
		draw_convex(line);
	}

	static void draw_circle(Vector2f center, float radius)
	{
		FloatConvex cir = FloatConvex::Circle(center, radius);
		draw_convex(cir);
	}

	static void draw_rectangle(Vector2f pos, Vector2f size)
	{
		FloatConvex rec = FloatConvex::Rectangle(pos, size);
		draw_convex(rec);
	}

	static void draw_text(Vector2f pos, std::string text, int size, sf::Color text_color, bool overlay = false)
	{
		sf::Text t;
		t.setString(text);
		int x = std::floorf(pos.x);
		int y = std::floorf(pos.y);
		t.setPosition(x, y);
		//t.setFillColor(fill_color);
		t.setFillColor(text_color);
		t.setOutlineThickness(1);
		t.setOutlineColor(sf::Color::Black);
		//t.setOutlineColor(outline_color);
		t.setFont(ResourceManager::Instance()->get_font("calibri-regular.ttf"));
		t.setCharacterSize(size);
		if (overlay)
			Renderer::Instance()->draw(&t);
		else
			Renderer::Instance()->draw(&t, view);
	}

private:
	static sf::View* view;

};

}