#include "pch.h"
#include "GameObject.h"
#include "Time.h"
#include "Renderer.h"
#include "Debug.h"
#include "Collisions.h"
#include "SceneManager.h"



#pragma region GUI

#include "ButtonComponent.h"

#pragma endregion
#pragma region Physics

#include "BoxColliderComponent.h"
#include "TilemapColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "RigidbodyComponent.h"

#pragma endregion
#pragma region Rendering

#include "SpriteComponent.h"
#include "AnimatedSpriteComponent.h"
#include "AnimationComponent.h"
#include "ChildAnimationComponent.h"
#include "TilemapComponent.h"
#include "CameraComponent.h"


#include "PlayerController.h"

#include "DontDestroyOnLoad.h"
#include "SceneChange.h"
#include "AudioSource.h"
#include "Inventory.h"
#include "InventoryWindow.h"
#include "InventoryGUIController.h"
#include "Interactor.h"
#include "IInteractable.h"
#include "Item.h"
#include "Chest.h"

namespace bm98
{
using namespace core;
GameObject::GameObject()
{
	unique_runtime_id = get_unique_id();
	init_variables();
}

GameObject::~GameObject()
{
	for (auto& c : components)
	{
		delete c;
	}
	components.clear();
}

void GameObject::init()
{
	init_components();
}

void GameObject::awake()
{
	set_world_position(position);

	awake_components();

}

void GameObject::start()
{
	initialized = true;
	start_components();
}

void GameObject::on_destroy()
{
	if (!active)
		return;
	for (auto& c : components)
	{
		c->on_destroy();
	}
}

void GameObject::update()
{
	for (auto& c_t_r : components_to_remove)
	{
		components.erase(std::find(components.begin(), components.end(), c_t_r));
		delete c_t_r;
	}
	components_to_remove.clear();
	
	if (!active)
		return;
	for(std::size_t i = 0; i != components.size(); i++)
		components[i]->update();
	

	//if(!parent)
		//transform.position = sprite.getPosition();
	
	//if (parent)
	//	transform.position = parent->transform.position + transform.local_position;
	
	//sprite.setPosition(transform.position);
	
}

void GameObject::late_update()
{
	if (!active)
		return;
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->late_update();
}

void GameObject::fixed_update()
{
	if (!active)
		return;
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->fixed_update();
}

void GameObject::on_collision_enter(Collision info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_collision_enter(info);
		return;
	}
	if (parent)
		parent->on_collision_enter(info);
}

void GameObject::on_collision_stay(Collision info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_collision_stay(info);
		return;
	}
	if (parent)
		parent->on_collision_stay(info);
	
}

void GameObject::on_collision_exit(Collision info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_collision_exit(info);
		return;
	}
	if (parent)
		parent->on_collision_exit(info);
	
}

void GameObject::on_trigger_enter(Collider info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_trigger_enter(info);
		return;
	}
	if (parent)
		parent->on_trigger_enter(info);
	
}

void GameObject::on_trigger_stay(Collider info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_trigger_stay(info);
		return;
	}
	if (parent)
		parent->on_trigger_stay(info);

}

void GameObject::on_trigger_exit(Collider info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_trigger_exit(info);
		return;
	}
	if (parent)
		parent->on_trigger_exit(info);
	
}

void GameObject::add_to_buffer(sf::View* view)
{
	if (!active)
	{
		return;
	}
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->add_to_buffer(view);

	for (auto& ch : children)
		ch->add_to_buffer(view);
	
}

std::vector<Editor::SerializedVar> GameObject::get_editor_values()
{

	std::vector<Editor::SerializedVar> variables;

	variables.push_back(Editor::SerializedVar("name", static_cast<void*>(&info.name), Editor::VarType::String));
	variables.push_back(Editor::SerializedVar("layer", static_cast<void*>(&info.layer),
		Editor::VarType::Dropdown, Sorting::ToVector()));
	variables.push_back(Editor::SerializedVar("position", static_cast<void*>(&position), Editor::VarType::Vector2f));
	variables.push_back(Editor::SerializedVar("local_position", static_cast<void*>(&local_position), Editor::VarType::Vector2f));

	return variables;
}

const bool& GameObject::is_active()
{
	return active;
}

void GameObject::set_active(bool active)
{
	this->active = active;
	for (std::size_t i = 0; i < components.size(); i++)
		components[i]->set_active(active);
	for (auto& ch : children)
		ch->set_active(active);
}

void GameObject::set_render(bool render)
{
	for (auto& c : components)
	{
		if (dynamic_cast<IRenderable*>(c))
			dynamic_cast<IRenderable*>(c)->set_render(render);
	}
}

void GameObject::set_parent(GameObject* parent)
{
	if (this->parent)
	{
		// remove as a child from previous parent if exists
		this->parent->remove_child(this);
	}

	EventSystem::Instance()->push_event(EventID::GAMEOBJECT_PARENT_CHANGE);
	this->parent = parent;

	if (this->parent)
	{
		// if I was a betting man... this statement is the true issue
		set_local_position(parent->get_world_position() - get_world_position());
		// 
		//seems to be the posiotion issue
		this->parent->add_child(this);
	}
}



void GameObject::remove_child(GameObject* child)
{
	children.erase(std::find(children.begin(), children.end(), child));
}

const size_t GameObject::get_unique_runtime_id() const
{
	return unique_runtime_id;
}

const GameObject::Info& GameObject::get_info() const
{
	return info;
}

const sf::Vector2f GameObject::get_center() const
{
	if (has_component<SpriteComponent>())
	{
		sf::FloatRect sc = get_component<SpriteComponent>().get_sprite().getGlobalBounds();
		return sf::Vector2f(
			sc.left + sc.width / 2,
			sc.top + sc.height / 2
		);
	}
	if (has_component<BoxColliderComponent>())
	{
		sf::FloatRect bc = get_component<BoxColliderComponent>().get_bounds();
		return sf::Vector2f(
			bc.left + bc.width / 2,
			bc.top + bc.height / 2
		);
	}
	return get_world_position();
}

GameObject* GameObject::get_parent()
{
	return parent;
}

std::vector<GameObject*> GameObject::get_all_relatives()
{
	std::vector<GameObject*> relatives;
	GameObject* p = get_parent();
	while (p)
	{
		relatives.push_back(p);
		p = p->get_parent();
	}

	std::vector<GameObject*> posterity = get_all_posterity();
	relatives.insert(relatives.end(), posterity.begin(), posterity.end());

	return relatives;
}

GameObject* GameObject::get_greatest_ancestor()
{
	GameObject* o = parent;
	while (o->get_parent())
	{
		o = o->get_parent();
	}
	return o;
}

std::vector<Component*> GameObject::get_components()
{
	return components;
}

std::vector<GameObject*> GameObject::get_children()
{
	return children;
}

std::vector<GameObject*> GameObject::get_all_posterity()
{
	std::vector<GameObject*> all_children = children;
	for (std::size_t i = 0; i != children.size(); i++)
	{
		std::vector<GameObject*> posterity = children[i]->get_all_posterity();
		all_children.insert(all_children.end(), posterity.begin(), posterity.end());
		
	}
	return all_children;
}

const bool GameObject::check_for_child(GameObject* game_object) const
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i] == game_object)
			return true;

	return false;
}

const bool GameObject::check_for_child(std::string name) const
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i]->get_info().name == name)
			return true;

	return false;
}

GameObject* GameObject::get_child(std::string name)
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i]->get_info().name == name)
			return children[i];
	return nullptr;
}

const bool GameObject::is_initialized() const
{
	return initialized;
}

#pragma region IDATA

Json::Value GameObject::serialize_json()
{
	Json::Value obj;

	obj["info"] = info.serialize_json();

	obj["transform"]["position"] = position.serialize_json();
	obj["transform"]["local-position"] = local_position.serialize_json();
	obj["transform"]["rotation"] = rotation;

	for (auto& c : components)
	{
		Json::Value obj2;
		obj2["name"] = typeid(*c).name();
		
		obj2["value"] = c->serialize_json();
		obj["components"].append(obj2);
	}
	for (auto& o : children)
	{
		obj["children"].append(o->serialize_json());
	}

	return obj;
}

void GameObject::unserialize_json(Json::Value obj)
{
	info.unserialize_json(obj["info"]);

	position.unserialize_json(obj["transform"]["position"]);
	local_position.unserialize_json(obj["transform"]["local-position"]);
	rotation = obj["transform"]["rotation"].asFloat();

	std::unordered_map<std::string, Json::Value> comps_data;
	for (Json::Value component : obj["components"])
	{
		if (component["name"].asString() == typeid(TilemapComponent).name())
		{
			comps_data[typeid(TilemapComponent).name()] = component["value"];
			add_component<TilemapComponent>();
		}
		else if (component["name"].asString() == typeid(TilemapColliderComponent).name())
		{
			comps_data[typeid(TilemapColliderComponent).name()] = component["value"];
			add_component<TilemapColliderComponent>();
		}
		else if (component["name"].asString() == typeid(SpriteComponent).name())
		{
			comps_data[typeid(SpriteComponent).name()] = component["value"];
			add_component<SpriteComponent>();
		}
		else if (component["name"].asString() == typeid(AnimatedSpriteComponent).name())
		{
			comps_data[typeid(AnimatedSpriteComponent).name()] = component["value"];
			add_component<AnimatedSpriteComponent>();
		}
		else if (component["name"].asString() == typeid(AnimationComponent).name())
		{
			comps_data[typeid(AnimationComponent).name()] = component["value"];
			add_component<AnimationComponent>();
		}
		else if (component["name"].asString() == typeid(BoxColliderComponent).name())
		{
			comps_data[typeid(BoxColliderComponent).name()] = component["value"];
			add_component<BoxColliderComponent>();
		}
		else if (component["name"].asString() == typeid(CapsuleColliderComponent).name())
		{
			comps_data[typeid(CapsuleColliderComponent).name()] = component["value"];
			add_component<CapsuleColliderComponent>();
		}
		else if (component["name"].asString() == typeid(ChildAnimationComponent).name())
		{
			comps_data[typeid(ChildAnimationComponent).name()] = component["value"];
			add_component<ChildAnimationComponent>();
		}
		else if (component["name"].asString() == typeid(RigidbodyComponent).name())
		{
			comps_data[typeid(RigidbodyComponent).name()] = component["value"];
			add_component<RigidbodyComponent>();
		}
		else if (component["name"].asString() == typeid(PlayerController).name())
		{
			comps_data[typeid(PlayerController).name()] = component["value"];
			add_component<PlayerController>();
		}
		else if (component["name"].asString() == typeid(ButtonComponent).name())
		{
			comps_data[typeid(ButtonComponent).name()] = component["value"];
			add_component<ButtonComponent>();
		}
		else if (component["name"].asString() == typeid(DontDestroyOnLoad).name())
		{
			comps_data[typeid(DontDestroyOnLoad).name()] = component["value"];
			add_component<DontDestroyOnLoad>();
		}
		else if (component["name"].asString() == typeid(SceneChange).name())
		{
			comps_data[typeid(SceneChange).name()] = component["value"];
			add_component<SceneChange>();
		}
		else if (component["name"].asString() == typeid(CameraComponent).name())
		{
			comps_data[typeid(CameraComponent).name()] = component["value"];
			add_component<CameraComponent>();
		}
		else if (component["name"].asString() == typeid(AudioSource).name())
		{
			comps_data[typeid(AudioSource).name()] = component["value"];
			add_component<AudioSource>();
		}
		else if (component["name"].asString() == typeid(Inventory).name())
		{
			comps_data[typeid(Inventory).name()] = component["value"];
			add_component<Inventory>();
		}
		else if (component["name"].asString() == typeid(InventoryWindow).name())
		{
			comps_data[typeid(InventoryWindow).name()] = component["value"];
			add_component<InventoryWindow>();
		}
		else if (component["name"].asString() == typeid(InventoryGUIController).name())
		{
			comps_data[typeid(InventoryGUIController).name()] = component["value"];
			add_component<InventoryGUIController>();
		}
		else if (component["name"].asString() == typeid(Interactor).name())
		{
			comps_data[typeid(Interactor).name()] = component["value"];
			add_component<Interactor>();
		}
		else if (component["name"].asString() == typeid(IInteractable).name())
		{
			comps_data[typeid(IInteractable).name()] = component["value"];
			add_component<IInteractable>();
		}
		else if (component["name"].asString() == typeid(Item).name())
		{
			comps_data[typeid(Item).name()] = component["value"];
			add_component<Item>();
		}
		else if (component["name"].asString() == typeid(Chest).name())
		{
		comps_data[typeid(Chest).name()] = component["value"];
		add_component<Chest>();
		}
	}

	for (auto& c : components)
	{
		c->unserialize_json(comps_data.at(typeid(*c).name()));
	}

	for (Json::Value child : obj["children"])
	{
		GameObject* go = new GameObject();
		go->set_parent(this);
		go->unserialize_json(child);
		SceneManager::Instance()->instantiate_gameobject_on_load(go);
	}

}

#pragma endregion

void GameObject::add_child(GameObject* child)
{
	children.push_back(child);
}

void GameObject::init_components()
{
	for (auto& c : components)
		c->init();
}

void GameObject::awake_components()
{
	for (auto& c : components)
		c->awake();
}

void GameObject::start_components()
{
	for (auto& c : components)
		c->start();
}

void GameObject::init_variables()
{
	active = true;
}

}