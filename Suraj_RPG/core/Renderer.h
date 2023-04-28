#pragma once
#include "../globals.h"
#include "../IRenderable.h"
#include "Math.h"
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
		RenderObject(sf::Drawable* drawable, bool& render, Sorting::Layer& sorting_layer,
			char& z_order, sf::View** view = nullptr, sf::Shader** shader = nullptr)
			:render(render), sorting_layer(sorting_layer), z_order(z_order)
		{
			this->sorting_layer = sorting_layer;
			this->drawable = drawable;
			this->z_order = z_order;
			this->view = view;
			this->shader = shader;
		}
		
		RenderObject(sf::Drawable* drawable, IRenderable* renderable)
			:render(renderable->get_render()), sorting_layer(renderable->get_sorting_layer()),
			z_order(renderable->get_z_order())
		{
			this->sorting_layer = renderable->get_sorting_layer();
			this->z_order = renderable->get_z_order();
			this->view = renderable->get_view_pointer();
			this->shader = renderable->get_shader_pointer();
			this->drawable = drawable;
		}

		Sorting::Layer& sorting_layer;
		bool& render;
		sf::Drawable* drawable;
		char& z_order;
		sf::View** view;
		sf::Shader** shader;

		// Will be used by ui elements
		bool has_global_bounds = false;

	};

	struct GizmoObject
	{
		GizmoObject(FloatConvex drawable)
		{
			this->drawable = drawable;
		}

		FloatConvex drawable;


	};
	void init(RenderTarget* render_target);

	inline bool is_top_ui(sf::Drawable* drawable) { return drawable == top_ui; }

	void add(RenderObject render_object);
	void add_ui(RenderObject ui_render_object);

	void add_gizmo(GizmoObject gizmo);

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
	Renderer(const Renderer& rhs)
	{

	}
	Renderer& operator=(const Renderer& rhs) {}

	RenderTarget* window;

	bool draw_gizmos = true;

	std::vector<GizmoObject> gizmos;

	std::list<RenderObject> renderables;
	std::list<RenderObject> ui_renderables;

	sf::Drawable* top_ui;


	// Inherited via RenderTarget
	virtual sf::Vector2u getSize() const override;

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

	void render();
	void render_list(std::list<RenderObject>& list);

	void render_gizmos();
	void clear();
	void clear_gizmos();
	void fixed_update();

	void update_top_ui();

	void sort();

};

}