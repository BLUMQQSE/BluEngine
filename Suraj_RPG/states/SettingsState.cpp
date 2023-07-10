#include "../pch.h"
#include "SettingsState.h"

#include "../core/Input.h"
#include "../core/Time.h"
#include "../core/Debug.h"
#include "../core/Physics.h"
#include "../core/Game.h"
#include "../core/ResourceManager.h"

#include "../GraphicsSettings.h"
#include "../gui/Gui.h"
namespace bm98
{
using namespace core;
SettingsState::SettingsState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
	: State(window, states, graphics_settings)
{
	state_name = "Settings_State";
	init_variables();
	init_fonts();
	init_text();
	init_gui();

	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(Color::Cyan);
	circle.setOutlineThickness(1);
	
	square.setFillColor(sf::Color::Transparent);
	square.setOutlineColor(Color::Cyan);
	square.setOutlineThickness(1);
	
	concave.setFillColor(sf::Color::Transparent);
	concave.setOutlineColor(Color::Cyan);
	concave.setOutlineThickness(1);

	std::vector<IRenderable> renders(3);
	for (int i = 0; i < 3; i++)
	{
		renders[i].set_render(true);
		renders[i].set_sorting_layer(options_layer);
		renders[i].set_z_order(z_order);
	}

	shapes.push_back(std::make_pair(&square, renders[0]));
	shapes.push_back(std::make_pair(&circle, renders[1]));
	shapes.push_back(std::make_pair(&concave, renders[2]));

	//Debug::init();
	Renderer::Instance()->add_ui(Renderer::RenderObject(&options_text, _render, options_layer, z_order));
	Renderer::Instance()->add_ui(Renderer::RenderObject(&circle, &shapes[0].second));
	Renderer::Instance()->add_ui(Renderer::RenderObject(&square, &shapes[1].second));

	Renderer::Instance()->add_ui(Renderer::RenderObject(&concave, &shapes[2].second));

	pb = std::make_unique<GUI::ProgressBar>();
	pb->set_position(Vector2f(800, 300));
	pb->set_size(Vector2f(300, 200));
	pb->set_percentage(.75f);
	pb->set_color(sf::Color::Magenta);

	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RENDERER_DRAW_GIZMOS_, this);

}

SettingsState::~SettingsState()
{
}

void SettingsState::init_state()
{
	Debug::Instance()->core_log("[SettingsState] Initialized", Debug::LogLevel::INFO);
}

void SettingsState::init_variables()
{
	modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::update_input()
{
	if (Input::Instance()->get_action_down("L"))
	{
		pb->set_percentage(.5f);
	}

	float delta = Time::Instance()->delta_time();
	if (Input::Instance()->get_action("W"))
		concave.move(0, -100 * delta);
	if (Input::Instance()->get_action("A"))
		concave.move(-100 * delta, 0);
	if (Input::Instance()->get_action("D"))
		concave.move(100 * delta, 0);
	if (Input::Instance()->get_action("S"))
		concave.move(0, 100 * delta);

	if (Input::Instance()->get_mouse_down(Input::Mouse::RIGHT))
		concave.rotate(10);

	if (buttons["BACK"]->is_pressed())
	{
		isRequestingQuit = true;
	}
	if (buttons["APPLY"]->is_pressed())
	{
		graphics_settings->resolution = modes[drop_downs["RESOLUTION"]->get_selected_index()];
		window->create(graphics_settings->resolution, graphics_settings->game_title, sf::Style::Default);
	}

	if (Input::Instance()->get_mouse_down())
	{
		if(square.contains_point(Input::Instance()->mouse_position()))
		{
			holding = true;
			offset = square.get_position()-Input::Instance()->mouse_position();
		}
	}
	if (Input::Instance()->get_mouse_up())
	{
		holding = false;
	}
}

void SettingsState::on_end_state()
{
	Debug::Instance()->core_log("[SettingsState] Shutdown", Debug::LogLevel::INFO);
}

void SettingsState::update()
{
	State::update();
	update_input();
	Debug::Instance()->mouse_position_display(font);


	if (holding)
	{
		square.set_position(Input::Instance()->mouse_position() + offset);
	}


	for (auto& it : buttons)
	{
		it.second->update();

	}
	for (auto& it2 : drop_downs)
	{
		it2.second->update();
	}
	flag_drop->update();

	for (auto& it3 : volume_sliders)
		it3.second->update();

	Game::get_game_settings().audio_settings.master_volume = volume_sliders.at("MASTER")->get_value() / 100.f;
	Game::get_game_settings().audio_settings.music_volume = volume_sliders.at("MUSIC")->get_value() / 100.f;
	Game::get_game_settings().audio_settings.sound_volume = volume_sliders.at("SOUND")->get_value() / 100.f;
	Game::get_game_settings().audio_settings.ambient_volume = volume_sliders.at("AMBIENT")->get_value() / 100.f;

	for (auto& v : volume_displays)
		v.second.second->set_text(std::to_string(static_cast<int>(std::round(volume_sliders.at(v.first)->get_value()))) + "%");
}

void SettingsState::update_sfml(sf::Event sfEvent)
{
}

void SettingsState::late_update()
{
}
void SettingsState::draw_gizmos()
{
	sf::VertexArray tris(sf::Triangles, rays.size() * 3);

	int vert_index = 0;
	for (int i = 0; i < rays.size(); i++)
	{
		int ii = (i + 1) % rays.size();
		
		tris[vert_index].position = std::get<0>(rays[i]);
		tris[vert_index+1].position = std::get<1>(rays[i]);
		tris[vert_index+2].position = std::get<1>(rays[ii]);
		tris[vert_index].color = sf::Color::White;
		tris[vert_index+1].color = sf::Color::White;
		tris[vert_index+2].color = sf::Color::White;
		vert_index += 3;
	}

	if (draw_vertex_arrays)
		Gizmo::draw_vertex_array(tris);
	if (draw_lines)
	{
		if (rays.size() > 0)
		{
			for (int i = 0; i < rays.size(); i++)
			{
				Gizmo::outline_color = Color::Transparent;
				Gizmo::fill_color = Color::Red;
				Gizmo::draw_line(std::get<0>(rays[i]), std::get<1>(rays[i]));

			}
		}
	}
}
void SettingsState::get_rays()
{
	rays.clear();
	ray_dirs.clear();
	Vector2f mp = Input::Instance()->mouse_position();
	
	// create all rays
	for (int i = 0; i < shapes.size(); i++)
	{
		for (int k = 0; k < shapes[i].first->getPointCount(); k++)
		{
			Vector2f dir = Vector2f(shapes[i].first->getPoint(k) - mp).get_normalized();
			
			// would also need the left
			Vector2f left = Vector2f((std::cos(-.0001) * dir.x) - (std::sin(-.0001) * dir.y),
				(std::sin(-.0001) * dir.x) + (std::cos(-.0001) * dir.y));
				
			// would also need the right
			Vector2f right = Vector2f((std::cos(.0001) * dir.x) - (std::sin(.0001) * dir.y),
										(std::sin(.0001) * dir.x) + (std::cos(.0001) * dir.y));
			
			ray_dirs.push_back(mp + left * 300.f);
			ray_dirs.push_back(mp + dir * 300.f);
			ray_dirs.push_back(mp + right * 300.f);
		}
	}
	for (int i = 0; i < light_bounds.getPointCount(); i+=2)
		ray_dirs.push_back(mp + Vector2f(light_bounds.getPoint(i) - mp).get_normalized() * 300.f);
	
	// check for closest intersect
	for (int i = 0; i < ray_dirs.size(); i++)
	{
		bool found_intersect = false;
		// change to infinity
		Vector2f closest_intersect = Vector2f::Infinity();
		for (int k = 0; k < shapes.size(); k++)
		{
			LineIntersect l = shapes[k].first->line_intersects(mp, ray_dirs[i]);
			if (l.collision_exists)
			{
				if (!found_intersect)
				{
					found_intersect = true;
					closest_intersect = l.get_contact_center();
				}
				else if (Vector2f::SqrDistance(mp, l.get_contact_center()) < Vector2f::SqrDistance(mp, closest_intersect))
				{
					found_intersect = true;
					closest_intersect = l.get_contact_center();
				}
			}
		}
		// If didn't hit anything, set to go to edge of light_bounds
		if (!found_intersect)
		{
			closest_intersect = ray_dirs[i];
		}
		rays.push_back(std::make_tuple(mp, closest_intersect, Vector2f::GetAngle(Vector2f::Up() + mp, mp, closest_intersect)));
	}
	
	// NEXT STEP: Sort all rays by their angle from center
	std::sort(rays.begin(), rays.end(), [](const std::tuple<Vector2f, Vector2f, float>& lhs, const std::tuple<Vector2f, Vector2f, float>& rhs) -> bool
	{
		return get<2>(lhs) > get<2>(rhs);
	});
	auto it = std::unique(rays.begin(), rays.end(),
						  [&](const std::tuple<Vector2f, Vector2f, float>& t1, const std::tuple<Vector2f, Vector2f, float>& t2)
						  {
							  return Vector2f::SqrDistance(std::get<1>(t1), std::get<1>(t2)) < 5;
						  });
	rays.resize(std::distance(rays.begin(), it));

}

void SettingsState::fixed_update()
{
	light_bounds.set_position(Input::Instance()->mouse_position());
	get_rays();
	Vector2f dif;
	dif = FloatConvex::Intersection(square, concave).penetration_vector;

	if (dif != Vector2f::Infinity())
	{
		square.move(dif.x, dif.y);
	}

}

void SettingsState::render()
{
}

void SettingsState::init_gui()
{
	init_buttons();
	init_drop_downs();
	init_volume_display();
}

void SettingsState::init_text()
{
	options_text.setFont(font);
	options_text.setPosition(sf::Vector2f(100, 100));
	options_text.setCharacterSize(30);
	options_text.setFillColor(sf::Color(255, 255, 255, 200));

	options_text.setString(
		"Resolution \n\nFullscreen \n\nVsync \n\nAntialiasing \n"
	);
}

void SettingsState::init_buttons()
{
	buttons["BACK"] = std::make_unique<GUI::Button>(100.f, 540.f, 150.f, 60.f, &font, "BACK", 28,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
	buttons["APPLY"] = std::make_unique<GUI::Button>(850.f, 540.f, 150.f, 60.f, &font, "APPLY", 28,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
}

void SettingsState::init_volume_display()
{
	volume_sliders["MASTER"] = std::make_unique<GUI::Slider>(700, 400, 400, 0, 100, 100, true);
	volume_sliders["MUSIC"] = std::make_unique<GUI::Slider>(700, 500, 400, 0, 100, 100, true);
	volume_sliders["SOUND"] = std::make_unique<GUI::Slider>(700, 600, 400, 0, 100, 100, true);
	volume_sliders["AMBIENT"] = std::make_unique<GUI::Slider>(700, 700, 400, 0, 100, 100, true);

	volume_displays["MASTER"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 400, 22, font, "MASTER", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("MASTER")->get_width() + 10, 400, 22, font, "100%", sf::Color::White));

	volume_displays["MUSIC"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 500, 22, font, "MUSIC", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("MUSIC")->get_width() + 10, 500, 22, font, "100%", sf::Color::White));
	volume_displays["SOUND"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 600, 22, font, "SOUND", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("SOUND")->get_width() + 10, 600, 22, font, "100%", sf::Color::White));
	volume_displays["AMBIENT"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 700, 22, font, "AMBIENT", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("AMBIENT")->get_width() + 10, 700, 22, font, "100%", sf::Color::White));

}


void SettingsState::init_drop_downs()
{
	std::vector<std::string> modes_str;
	for (auto& i : modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}
	drop_downs["RESOLUTION"] = std::make_unique<GUI::DropDownList>(100, 100, 150, 30, font,
		modes_str);

	flag_drop = std::make_unique<GUI::FlagDropDownList>(600, 100, 150, 30, font, modes_str);

	//drop_downs["FULLSCREEN"] = new GUI::DropDownList(800, 175, 150, 30, font, screen_list, 2);
}
void SettingsState::handle_event(Event* event)
{
	if (event->get_event_id() == EventID::_SYSTEM_RENDERER_DRAW_GIZMOS_)
	{
		draw_gizmos();
	}
}
}