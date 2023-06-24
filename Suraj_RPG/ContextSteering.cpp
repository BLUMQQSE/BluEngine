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
	if(enemy_data.lock())
		enemy_data = core::ResourceManager::Instance()->get_data_asset<EnemyData>(enemy_data_id);
	if (current_target_id > 0)
		current_target = SceneManager::Instance()->find_with_id(current_target_id, game_object->self());

	if(current_target.lock())
		target_mask[current_target.lock()->get_info().layer] = true;

}

void ContextSteering::fixed_update()
{
	std::vector<std::weak_ptr<ColliderComponent>> colliders_in_range;
	int result = core::Physics::Instance()->OverlapCircle(game_object->get_center(), enemy_data.lock()->get_detection_radius(), target_mask,
														  game_object, colliders_in_range);
	if (result == 0)
		return;

	bool target_found = false;
	for (std::size_t i = 0; i < result; i++)
	{
		if (colliders_in_range[i].lock()->get_game_object()->self() == current_target.lock())
		{
			target_found = true;
			break;
		}
	}

	if (target_found)
	{
		Vector2f raycast_dir = current_target.lock()->get_center() - game_object->get_center();
		std::shared_ptr<Physics::RayHit> ray;
		bool x = Physics::Instance()->raycast(game_object->get_center(), raycast_dir.get_normalized(), game_object->self(), enemy_data.lock()->get_detection_radius(),
											  enemy_data.lock()->get_detection_mask(), ray);

		if (x)
		{
			if (ray->collider.lock() == current_target.lock()->get_component_of_type<ColliderComponent>().lock())
			{
				// CAN MOVE TOWARDS TARGET AS ITS THE FIRST HIT
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


}