#include "pch.h"
#include "TestClass.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Math.h"
namespace bm98
{
using namespace core;
std::string prefab = "sword.json";

void bm98::TestScript::start()
{
	GameObject* prefab_object = new GameObject();
	prefab_object->unserialize_json(ResourceManager::get_prefab_data(prefab));
	SceneManager::instantiate_gameobject(prefab_object);
	
	
	
}

}