#include "pch.h"
#include "ContextSteering.h"
#include "GameObject.h"
#include "core/ResourceManager.h"
#include "core/Physics.h"
#include "core/SceneManager.h"
#include "data_assets/DataAssets.h"

namespace bm98
{




void ContextSteering::init()
{
}

void ContextSteering::awake()
{
	if(!enemy_data.lock())
		enemy_data = core::ResourceManager::Instance()->get_data_asset<EnemyData>(enemy_data_id);
	
	current_target = SceneManager::Instance()->find_with_tag(Tags::Tag::PLAYER, game_object->self());
	
	//if (current_target_id > 0)
	//	current_target = SceneManager::Instance()->find_with_id(current_target_id, game_object->self());

	//if(current_target.lock())
	//	target_mask[current_target.lock()->get_info().layer] = true;

}

void ContextSteering::fixed_update()
{
	if (!current_target.lock())
		return;

	target_in_range = false;

	std::vector<std::weak_ptr<ColliderComponent>> colliders_in_range;
	int result = core::Physics::Instance()->OverlapCircle(game_object->get_physical_center(), enemy_data.lock()->get_detection_radius(), enemy_data.lock()->get_target_mask(),
														  game_object, colliders_in_range);

	for (std::size_t i = 0; i < result; i++)
	{
		if (colliders_in_range[i].lock()->get_game_object()->self() == current_target.lock())
		{
			target_in_range = true;
			break;
		}
	}

	if (!target_in_range)
		following = false;

	obstacles.clear();
	core::Physics::Instance()->OverlapCircle(game_object->get_physical_center(), enemy_data.lock()->get_detection_radius(), enemy_data.lock()->get_obstacle_mask(),
														  game_object, obstacles);

	detect_dangers();
	if (result == 0)
	{
		
		return;
	}


	if (target_in_range)
	{
		detect_interests();

		Vector2f raycast_dir = current_target.lock()->get_physical_center() - game_object->get_physical_center();
		std::shared_ptr<Physics::RayHit> ray = std::make_shared<Physics::RayHit>();
		
		PhysicsNS::LayerMask mask = enemy_data.lock()->get_target_mask();
		mask += enemy_data.lock()->get_obstacle_mask();
		bool hit = Physics::Instance()->raycast(game_object->get_physical_center(), raycast_dir.get_normalized(), game_object->self(), enemy_data.lock()->get_detection_radius(),
											  mask, ray);

		if (hit)
		{
			if (ray->collider.lock() == current_target.lock()->get_component_of_type<ColliderComponent>().lock())
			{
				calculate_direction_to_move();
				game_object->get_component<RigidbodyComponent>().lock()->move(result_direction);
				// CAN MOVE TOWARDS TARGET AS ITS THE FIRST HIT
				//std::cout << "can move towards player\n";
				object_unblocked = true;
			}
			else
			{
				object_unblocked = false;
				//std::cout << "player is blocked\n";
			}
		}

	}

}

std::vector<Editor::SerializedVar> ContextSteering::get_editor_values()
{
	std::vector<Editor::SerializedVar> result;
	result.push_back(Editor::SerializedVar("enemy_data_id", &enemy_data_id, Var::Type::String));
	return result;
}

void ContextSteering::on_draw_gizmos()
{
	if (!current_target.lock())
		return;
	if (target_in_range)
	{
		if (object_unblocked)
			Gizmo::fill_color = Color::LimeGreen;
		else
			Gizmo::fill_color = Color::Red;
		Gizmo::outline_thickness = 0;
		//Gizmo::draw_line(game_object->get_physical_center(), current_target.lock()->get_physical_center());
	}
	
	Gizmo::fill_color = Color::Transparent;
	Gizmo::outline_color = Color::Charcoal;
	Gizmo::outline_thickness = 1;
	Gizmo::draw_circle(game_object->get_physical_center(), enemy_data.lock()->get_detection_radius());
	Gizmo::fill_color = Color::Transparent;
	Gizmo::outline_color = Color::Grey;
	Gizmo::draw_circle(game_object->get_physical_center(), target_reached_threshold);
	
	for (int i = 0; i < danger.size(); i++)
	{
		if (danger[i] == 0.f)
			continue;
		Gizmo::fill_color = Color::Red;
		Gizmo::outline_thickness = 0;
		int danger_increase_for_view = 25;
		Gizmo::draw_line(game_object->get_physical_center(), game_object->get_physical_center() + Vector2f(Orientation::GetDirections()[i] * (danger[i] * danger_increase_for_view)));
	}
	for (int i = 0; i < interest.size(); i++)
	{
		if (interest[i] == 0.f)
			continue;
		Gizmo::fill_color = Color::LimeGreen;
		Gizmo::outline_thickness = 0;
		int interest_increase_for_view = 25;
		Gizmo::draw_line(game_object->get_physical_center(), game_object->get_physical_center() + Vector2f(Orientation::GetDirections()[i] * (interest[i] * interest_increase_for_view)));
	}
}

void ContextSteering::unserialize_json(Json::Value obj)
{
	current_target_id = obj["current-target-id"].asInt64();
	enemy_data_id = obj["enemy-data-id"].asString();
}

Json::Value ContextSteering::serialize_json()
{
	Json::Value obj;

	obj["current-target-id"] = (uint64_t)current_target_id;
	obj["enemy-data-id"] = enemy_data_id;

	return obj;
}

void ContextSteering::detect_dangers()
{
	danger.fill(0.f);
	for (int i = 0; i < obstacles.size(); i++)
	{
		Vector2f dir_to_obstacle = obstacles[i].lock()->get_closest_point(game_object->get_physical_center()) - game_object->get_physical_center();
		float dist_to_obstacle = dir_to_obstacle.magnitude();

		// Assign a weight to the obstacle based on how close to it we are
		float weight = 0;
		if (dist_to_obstacle <= obstacle_buffer_space)
			weight = 1;
		else
			weight = (enemy_data.lock()->get_detection_radius() - dist_to_obstacle) / enemy_data.lock()->get_detection_radius();

		// Check every direction to see if is acts as the closest danger in that direction
		for (int i = 0; i < Orientation::GetDirections().size(); i++)
		{
			float result = Vector2f::DotProduct(dir_to_obstacle.get_normalized(), Orientation::GetDirections()[i]);

			float value_to_put_in = result * weight;

			if (value_to_put_in > danger[i])
				danger[i] = value_to_put_in;
		}

	}
}

void ContextSteering::calculate_direction_to_move()
{
	std::array<float, 8> result;
	for (int i = 0; i < 8; i++)
	{
		result[i] = interest[i] - danger[i];
		if (result[i] < 0)
			result[i] = 0;
	}
	result_direction = Vector2f::Zero();
	for (int i = 0; i < 8; i++)
	{
		result_direction += Orientation::GetDirections()[i] * result[i];
	}
	Vector2f::Normalize(result_direction);
}

void ContextSteering::detect_interests()
{
	interest.fill(0.0f);
	if (reached_last_target)
	{
		//fill in later
		return;
	}

	if (current_target.lock() && target_in_range)
		target_position_cached = current_target.lock()->get_physical_center();

	if (Vector2f::SqrDistance(game_object->get_physical_center(), target_position_cached) < std::pow(target_reached_threshold, 2))
	{
		reached_last_target = true;
	}

	Vector2f dir_to_target = target_position_cached - game_object->get_physical_center();
	for (int i = 0; i < interest.size(); i++)
	{
		float result = Vector2f::DotProduct(dir_to_target.get_normalized(), Orientation::GetDirections()[i]);

		if (result > interest[i])
			interest[i] = result;

	}

}


}