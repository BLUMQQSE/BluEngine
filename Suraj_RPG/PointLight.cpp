#include "pch.h"
#include "PointLight.h"
#include "GameObject.h"
#include "core/Physics.h"
#include "SpriteComponent.h"
#include "core/SceneManager.h"
#include "TilemapComponent.h"
namespace bm98
{

void PointLight::init()
{;
	light_shader = ResourceManager::Instance()->get_shader(shader_file_name);
	// catch case for editor
	if (!light_shader)
		return;
	light_collider = game_object->get_component_of_type<ColliderComponent>();
	if (!light_collider.lock())
		return;

	light_shader->setUniform("hasTexture", true);
	light_shader->setUniform("intensity", 0.5f);
	light_shader->setUniform("lightPos", game_object->get_physical_center());
	light_shader->setUniform("lightColor", sf::Glsl::Vec4(color));

	radius *= UNIT_SIZE;
	last_fixed_pos = game_object->get_physical_center();

	set_sorting_layer(Sorting::Layer::SHADER);
	set_render(true);
	add_shader(std::to_string(game_object->get_info().unique_id)
			   + shader_file_name, light_shader);
	set_view(SceneManager::Instance()->get_active_scene_view());

	radius = light_collider.lock()->get_collider_bounds().getPoint(1).x - light_collider.lock()->get_collider_bounds().getPoint(0).x;
	
	//Renderer::Instance()->add(Renderer::RenderObject(&tris, this));
	
	std::weak_ptr<GameObject> g = SceneManager::Instance()->find_with_tag(Tags::Tag::TILEMAP, game_object->self());
	if (g.lock())
		g.lock()->get_component<TilemapComponent>().lock()->temp_add_shader(light_shader);

}

void PointLight::awake()
{

}

void PointLight::update()
{
}


void PointLight::fixed_update()
{      
	light_shader->setUniform("lightPos", game_object->get_physical_center());
	check_positions();
	if (update_light)
	{
		rays.clear();
		tris.clear();
		update_light = false;
		calculate_rays();
		calculate_triangles();
	}
}

void PointLight::on_trigger_enter(Collider info)
{
	std::weak_ptr<ColliderComponent> col = info.game_object.lock()->get_component_of_type<ColliderComponent>();
	if (col.lock()->blocks_light())
	{
		//add to light blockers
		light_blocker_colliders.push_back(std::make_pair(col, col.lock()->get_game_object()->get_physical_center()));
		update_light = true;
	}
	else
	{
		//add to light absorbers
		light_absorber_colliders.push_back(std::make_pair(col, col.lock()->get_game_object()->get_physical_center()));
		update_light = true;
	}

	if (col.lock()->get_game_object()->has_component<SpriteComponent>())
	{
		col.lock()->get_game_object()->get_component<SpriteComponent>().lock()->add_shader(std::to_string(game_object->get_info().unique_id)
																									+ shader_file_name, light_shader);
		col.lock()->get_game_object()->get_component<SpriteComponent>().lock()->set_bool_uniform(std::to_string(game_object->get_info().unique_id)
																										  + shader_file_name, "hasTexture", true);
	}
}

void PointLight::on_trigger_exit(Collider info)
{
	std::pair<std::weak_ptr<ColliderComponent>, Vector2f> pair = std::make_pair(info.game_object.lock()->get_component_of_type<ColliderComponent>(),
																				info.game_object.lock()->get_physical_center());
	if (pair.first.lock()->blocks_light())
	{
		auto it = std::find_if(light_blocker_colliders.begin(), light_blocker_colliders.end(), [&pair](const std::pair<std::weak_ptr<ColliderComponent>, Vector2f>& ptr1)
			{
			return ptr1.first.lock() == pair.first.lock();
			});
		if (it != light_blocker_colliders.end())
		{
			light_blocker_colliders.erase(it);
			update_light = true;
		}
	}
	else
	{
		auto it = std::find_if(light_absorber_colliders.begin(), light_absorber_colliders.end(), [&pair](const std::pair<std::weak_ptr<ColliderComponent>, Vector2f>& ptr1)
							   {
								   return ptr1.first.lock() == pair.first.lock();
							   });
		if (it != light_absorber_colliders.end())
		{
			light_absorber_colliders.erase(it);
			update_light = true;
		}
	}
	if (pair.first.lock()->get_game_object()->has_component<SpriteComponent>())
	{
		pair.first.lock()->get_game_object()->get_component<SpriteComponent>().lock()->remove_shader(std::to_string(game_object->get_info().unique_id)
																						   + shader_file_name);
	}
}

void PointLight::on_draw_gizmos()
{
	if (rays.size() > 0)
	{
		Gizmo::outline_color = Color::Transparent;
		Gizmo::fill_color = Color::Red;
		for (int i = 0; i < rays.size(); i++)
		{
			Gizmo::draw_line(std::get<0>(rays[i]), std::get<1>(rays[i]));
		}
	}
	Gizmo::outline_color = Color::Yellow;
	Gizmo::fill_color = Color::Transparent;
	Gizmo::outline_thickness = 1;

	//Gizmo::draw_vertex_array(tris, light_shader);
}

Json::Value PointLight::serialize_json()
{
	Json::Value obj;
	
	obj[RemoveNamespace(typeid(IRenderable).name())] = IRenderable::serialize_json();
	obj["intensity"] = intensity;
	obj["color"]["r"] = color.r;
	obj["color"]["g"] = color.g;
	obj["color"]["b"] = color.b;
	obj["color"]["a"] = color.a;
	obj["shader-file-name"] = shader_file_name;

	return obj;
}

void PointLight::unserialize_json(Json::Value obj)
{
	IRenderable::unserialize_json(obj[RemoveNamespace(typeid(IRenderable).name())]);
	intensity = obj["intensity"].asFloat();
	color.r = obj["color"]["r"].asFloat();
	color.g = obj["color"]["g"].asFloat();
	color.b = obj["color"]["b"].asFloat();
	color.a = obj["color"]["a"].asFloat();
	shader_file_name = obj["shader-file-name"].asString();
}

std::vector<Editor::SerializedVar> PointLight::get_editor_values()
{
	std::vector<Editor::SerializedVar> result;

	result.push_back(Editor::SerializedVar("radius", (void*)&radius, Var::Type::Float));
	result.push_back(Editor::SerializedVar("intensity", (void*)&intensity, Var::Type::Float));
	result.push_back(Editor::SerializedVar("shader_file_name", (void*)&shader_file_name, Var::Type::String));

	return result;
}

void PointLight::calculate_rays()
{
	if (!light_collider.lock())
		return;
	std::vector<Vector2f> ray_dirs;
	Vector2f center = game_object->get_physical_center();

	// create all rays
	for (int i = 0; i < light_blocker_colliders.size(); i++)
	{
		for (int k = 0; k < light_blocker_colliders[i].first.lock()->get_collider_bounds().getPointCount(); k++)
		{
			Vector2f dir = Vector2f(light_blocker_colliders[i].first.lock()->get_collider_bounds().getPoint(k) - center).get_normalized();

			// would also need the left
			Vector2f left = Vector2f((std::cos(-.0001) * dir.x) - (std::sin(-.0001) * dir.y),
									 (std::sin(-.0001) * dir.x) + (std::cos(-.0001) * dir.y));

			// would also need the right
			Vector2f right = Vector2f((std::cos(.0001) * dir.x) - (std::sin(.0001) * dir.y),
									  (std::sin(.0001) * dir.x) + (std::cos(.0001) * dir.y));

			ray_dirs.push_back(center + left * radius);
			ray_dirs.push_back(center + dir * radius);
			ray_dirs.push_back(center + right * radius);
		}
	}
	for (int i = 0; i < light_collider.lock()->get_collider_bounds().getPointCount(); i++)
		ray_dirs.push_back(center + Vector2f(light_collider.lock()->get_collider_bounds().getPoint(i) - center).get_normalized() * radius);

	// check for closest intersect
	for (int i = 0; i < ray_dirs.size(); i++)
	{
		bool found_intersect = false;
		// change to infinity
		Vector2f closest_intersect = Vector2f::Infinity();
		for (int k = 0; k < light_blocker_colliders.size(); k++)
		{
			LineIntersect l = light_blocker_colliders[k].first.lock()->get_collider_bounds().line_intersects(center, ray_dirs[i]);
			if (l.collision_exists)
			{
				if (!found_intersect)
				{
					found_intersect = true;
					closest_intersect = l.get_contact_center();
				}
				else if (Vector2f::SqrDistance(center, l.get_contact_center()) < Vector2f::SqrDistance(center, closest_intersect))
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
		rays.push_back(std::make_tuple(center, closest_intersect, Vector2f::GetAngle(Vector2f::Up() + center, center, closest_intersect)));
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

void PointLight::calculate_triangles()
{
	tris = sf::VertexArray(sf::Triangles, rays.size() * 3);
	int vert_index = 0;
	for (int i = 0; i < rays.size(); i++)
	{
		int ii = (i + 1) % rays.size();

		tris[vert_index].position = std::get<0>(rays[i]);
		tris[vert_index + 1].position = std::get<1>(rays[i]);
		tris[vert_index + 2].position = std::get<1>(rays[ii]);
		tris[vert_index].color = sf::Color::White;
		tris[vert_index + 1].color = sf::Color::White;
		tris[vert_index + 2].color = sf::Color::White;
		vert_index += 3;
	}
}

void PointLight::check_positions()
{
	if (game_object->get_physical_center() != last_fixed_pos)
	{
		last_fixed_pos = game_object->get_physical_center();
		update_light = true;
	}
	auto it = light_blocker_colliders.begin();
	for (it; it != light_blocker_colliders.end(); ++it)
	{
		if (it->first.expired())
		{
			if (std::next(it) == light_blocker_colliders.end())
			{
				light_blocker_colliders.erase(it);
				break;
			}
			else
			{
				it = light_blocker_colliders.erase(it);
				continue;
			}
		}
		if (it->first.lock()->get_game_object()->get_physical_center() != it->second)
		{
			it->second = it->first.lock()->get_game_object()->get_physical_center();
			update_light = true;
		}
	}
	it = light_absorber_colliders.begin();
	for (it; it != light_absorber_colliders.end(); ++it)
	{
		if (it->first.expired())
		{
			if (std::next(it) == light_blocker_colliders.end())
			{
				light_absorber_colliders.erase(it);
				break;
			}
			else
			{
				it = light_absorber_colliders.erase(it);
				continue;
			}
		}
		if (it->first.lock()->get_game_object()->get_physical_center() != it->second)
		{
			it->second = it->first.lock()->get_game_object()->get_physical_center();
		}
	}
}


}