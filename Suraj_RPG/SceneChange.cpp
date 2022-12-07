#include "pch.h"
#include "SceneChange.h"
#include "Collisions.h"
namespace bm98
{
SceneChange::SceneChange(float pos_x, float pos_y, std::string scene_name, float spawn_x, float spawn_y)
{
	//info.tag = Tag::SCENECHANGE;
}

SceneChange::~SceneChange()
{
}

void SceneChange::on_collision_enter(Collision info)
{
	if (info.game_object->get_info().tag == Tag::PLAYER)
	{
		/*
		Player* sc = dynamic_cast<Player*>(info.game_object);
		if (sc)
		{
			std::cout << "Change scene here\n";
		}
		*/
	}
}

std::string SceneChange::get_scene()
{
	return scene_name;
}

sf::Vector2f SceneChange::get_scene_destination()
{
	return scene_destination;
}
}


