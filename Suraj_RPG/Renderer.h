#pragma once
#include "globals.h"
#include "IRenderable.h"
#include <limits>
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
		RenderObject(sf::Drawable* drawable, bool& render, SortingLayer& sorting_layer,
			char& z_order, sf::View** view = nullptr, sf::Shader** shader = nullptr)
			:render(render), sorting_layer(sorting_layer), z_order(z_order)
		{
			this->sorting_layer = sorting_layer;
			this->drawable = drawable;
			this->z_order = z_order;
			this->view = view;
			this->shader = shader;
			this->private_entry_key = Renderer::Instance()->get_id();
			Renderer::Instance()->Instance()->increase_id();
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
			this->private_entry_key = Renderer::Instance()->get_id();
			Renderer::Instance()->increase_id();
		}

		SortingLayer& sorting_layer;
		bool& render;
		sf::Drawable* drawable;
		char& z_order;
		sf::View** view;
		sf::Shader** shader;
		unsigned private_entry_key;

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

	void add(RenderObject render_object);
	void add_gizmo(GizmoObject gizmo);
	void remove(sf::Drawable* drawable);
	void remove(RenderObject render_object);

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

	void set_view(sf::View view = Instance()->window->getDefaultView());
	//static const bool& top_ui(const sf::Drawable& drawable);
	//static void add_tile(RenderObject render_object);
	// should run once on state change

	sf::Vector2u get_window_size();


protected:

private:

	Renderer();
	~Renderer() { }
	Renderer(const Renderer& rhs)
	{

	}
	Renderer& operator=(const Renderer& rhs) {}

	RenderTarget* window;

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

	unsigned id = 0;
	bool draw_gizmos = true;
	std::set<RenderObject, cmpStruct> render_objects;
	std::vector<GizmoObject> gizmos;

	//static sf::Drawable* _top_ui;


	// Inherited via RenderTarget
	virtual sf::Vector2u getSize() const override;

	const unsigned& get_id();
	void increase_id();

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

	void render();
	void render_gizmos();
	void clear();

	void fixed_update();

};

}