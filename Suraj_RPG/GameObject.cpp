#include "pch.h"
#include "GameObject.h"
#include "core/Time.h"
#include "core/Renderer.h"
#include "core/Debug.h"
#include "Collisions.h"
#include "core/SceneManager.h"

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
#include "CombatInventory.h"
#include "InventoryWindow.h"
#include "InventoryGUIController.h"
#include "Interactor.h"
#include "IInteractable.h"
#include "ItemController.h"
#include "WeaponController.h";
#include "Chest.h"

#include "DummyAI.h"

namespace bm98
{
using namespace core;
GameObject::GameObject()
{
	unique_runtime_id = get_unique_id();
	active = true;
	//parent = nullptr;
}

GameObject::~GameObject()
{
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

	for (auto& c : components)
	{
		delete c;
	}
	components.clear();
}

void GameObject::update()
{
	handle_removed_components();
	
	if (!active)
		return;
	for(std::size_t i = 0; i != components.size(); i++)
		components[i]->update();
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
	if (parent.lock())
		parent.lock()->on_collision_enter(info);
}

void GameObject::on_collision_stay(Collision info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_collision_stay(info);
		return;
	}
	if (parent.lock())
		parent.lock()->on_collision_stay(info);
}

void GameObject::on_collision_exit(Collision info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_collision_exit(info);
		return;
	}
	if (parent.lock())
		parent.lock()->on_collision_exit(info);
}

void GameObject::on_trigger_enter(Collider info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_trigger_enter(info);
		return;
	}
	if (parent.lock())
		parent.lock()->on_trigger_enter(info);
}

void GameObject::on_trigger_stay(Collider info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_trigger_stay(info);
		return;
	}
	if (parent.lock())
		parent.lock()->on_trigger_stay(info);
}

void GameObject::on_trigger_exit(Collider info)
{
	if (has_component<RigidbodyComponent>())
	{
		for (std::size_t i = 0; i != components.size(); i++)
			components[i]->on_trigger_exit(info);
		return;
	}
	if (parent.lock())
		parent.lock()->on_trigger_exit(info);
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
		ch.lock()->add_to_buffer(view);
}

std::vector<Editor::SerializedVar> GameObject::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;

	variables.push_back(Editor::SerializedVar("name", static_cast<void*>(&info.name), Var::Type::String));
	variables.push_back(Editor::SerializedVar("layer", static_cast<void*>(&info.layer),
		Var::Type::Dropdown, Sorting::ToVector()));
	variables.push_back(Editor::SerializedVar("position", static_cast<void*>(&position), Var::Type::Vector2f));
	variables.push_back(Editor::SerializedVar("local_position", static_cast<void*>(&local_position), 
		Var::Type::Vector2f));

	return variables;
}

void GameObject::editor_update()
{
	Renderer::Instance()->refresh();
	for (std::size_t i = 0; i < components.size(); i++)
		components[i]->editor_update();
	set_world_position(position);
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
		ch.lock()->set_active(active);
}

void GameObject::set_render(bool render)
{
	for (auto& c : components)
	{
		if (dynamic_cast<IRenderable*>(c))
			dynamic_cast<IRenderable*>(c)->set_render(render);
	}
}

void GameObject::set_parent(std::shared_ptr<GameObject> parent)
{
	if (this->parent.lock())
	{
		std::cout << this->parent.lock()->get_info().name << "\n";
		// remove as a child from previous parent if exists
		this->parent.lock()->remove_child(this->self());
	}

	EventSystem::Instance()->push_event(EventID::GAMEOBJECT_PARENT_CHANGE);
	this->parent = parent;

	if (this->parent.lock())
	{
		// if I was a betting man... this statement is the true issue
		set_local_position(parent->get_world_position() - get_world_position());
		// 
		//seems to be the posiotion issue
		this->parent.lock()->add_child(this->self());
	}
}

void GameObject::remove_child(std::shared_ptr<GameObject> child)
{
	std::vector<std::weak_ptr<GameObject>>::iterator obj = std::find_if(children.begin(), children.end(),
																		  [child](std::weak_ptr<GameObject> const& i) { return i.lock().get() == child.get(); });

	children.erase(obj);
}

const size_t GameObject::get_unique_runtime_id() const
{
	return unique_runtime_id;
}

GameObject::Info& GameObject::get_info()
{
	return info;
}

Vector2f GameObject::get_center()
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

std::weak_ptr<GameObject> GameObject::get_parent()
{
	return parent;
}

std::vector<std::weak_ptr<GameObject>> GameObject::get_all_relatives()
{
	std::vector<std::weak_ptr<GameObject>> relatives;
	std::shared_ptr<GameObject> p = get_parent().lock();
	while (p)
	{
		relatives.push_back(p);
		p = p->get_parent().lock();
	}

	std::vector<std::weak_ptr<GameObject>> posterity = get_all_posterity();
	relatives.insert(relatives.end(), posterity.begin(), posterity.end());

	return relatives;
}

std::weak_ptr<GameObject> GameObject::get_greatest_ancestor()
{
	std::shared_ptr<GameObject> o = parent.lock();
	while (o->get_parent().lock())
	{
		o = o->get_parent().lock();
	}
	return o;
}

std::vector<Component*> GameObject::get_components()
{
	return components;
}

std::vector<std::weak_ptr<GameObject>> GameObject::get_children()
{
	return children;
}

std::vector<std::weak_ptr<GameObject>> GameObject::get_all_posterity()
{
	std::vector<std::weak_ptr<GameObject>> all_children = children;
	for (std::size_t i = 0; i != children.size(); i++)
	{
		std::vector<std::weak_ptr<GameObject>> posterity = children[i].lock()->get_all_posterity();
		all_children.insert(all_children.end(), posterity.begin(), posterity.end());
		
	}
	return all_children;
}

const bool GameObject::check_for_child(std::shared_ptr<GameObject> game_object) const
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i].lock() == game_object)
			return true;

	return false;
}

const bool GameObject::check_for_child(std::string name) const
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i].lock()->get_info().name == name)
			return true;

	return false;
}

std::weak_ptr<GameObject> GameObject::get_child(std::string name)
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i].lock()->get_info().name == name)
			return children[i];
	
	return std::weak_ptr<GameObject>();
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
		obj2["name"] = RemoveNamespace(typeid(*c).name());
		
		obj2["value"] = c->serialize_json();
		obj["components"].append(obj2);
	}
	for (auto& o : children)
	{
		obj["children"].append(o.lock()->serialize_json());
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
		add_component_by_name(component["name"].asString(), component["value"], &comps_data);
	}

	for (auto& c : components)
		c->unserialize_json(comps_data.at(RemoveNamespace(typeid(*c).name())));
	

	for (Json::Value child : obj["children"])
	{
		std::shared_ptr<GameObject> go = std::make_shared<GameObject>();

		go->set_parent(this->self());
		go->unserialize_json(child);
		SceneManager::Instance()->instantiate_gameobject_on_load(go);
	}

}

#pragma endregion

void GameObject::add_child(std::shared_ptr<GameObject> child)
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

Component* GameObject::add_component_by_name(std::string component_name, Json::Value component_data,
											 std::unordered_map<std::string, Json::Value>* data_map)
{
	if (data_map)
		data_map->insert(std::make_pair(component_name, component_data));
	
#pragma region A
	if (component_name == "AnimatedSpriteComponent")
		return &add_component<AnimatedSpriteComponent>();
	if (component_name == "AnimationComponent")
		return &add_component<AnimationComponent>();
	if (component_name == "AudioSource")
		return &add_component<AudioSource>();
#pragma endregion
#pragma region B
	if (component_name == "BoxColliderComponent")
		return &add_component<BoxColliderComponent>();
	if (component_name == "ButtonComponent")
		return &add_component<ButtonComponent>();
#pragma endregion
#pragma region C
	if (component_name == "CameraComponent")
	return &add_component<CameraComponent>();
	if (component_name == "CapsuleColliderComponent")
		return &add_component<CapsuleColliderComponent>();
	if (component_name == "Chest")
		return &add_component<Chest>();
	if (component_name == "ChildAnimationComponent")
		return &add_component<ChildAnimationComponent>();
	if (component_name == "CombatInventory")
		return &add_component<CombatInventory>();
#pragma endregion
#pragma region D
	if (component_name == "DontDestroyOnLoad")
		return &add_component<DontDestroyOnLoad>();
	if (component_name == "DummyAI")
		return &add_component<DummyAI>();
#pragma endregion
#pragma region I
	if (component_name == "IInteractable")
		return &add_component<IInteractable>();
	if (component_name == "Inventory")
		return &add_component<Inventory>();
	if (component_name == "InventoryGUIController")
		return &add_component<InventoryGUIController>();
	if (component_name == "InventoryWindow")
		return &add_component<InventoryWindow>();
	if (component_name == "Interactor")
		return &add_component<Interactor>();
	if (component_name == "ItemController")
		return &add_component<ItemController>();
#pragma endregion
#pragma region P
	if (component_name == "PlayerController")
		return &add_component<PlayerController>();
#pragma endregion
#pragma region R
	if (component_name == "RigidbodyComponent")
		return &add_component<RigidbodyComponent>();
#pragma endregion
#pragma region S
	if (component_name == "SceneChange")
		return &add_component<SceneChange>();
	if (component_name == "SpriteComponent")
		return &add_component<SpriteComponent>();
#pragma endregion
#pragma region T
	if (component_name == "TilemapColliderComponent")
		return &add_component<TilemapColliderComponent>();
	if (component_name == "TilemapComponent")
		return &add_component<TilemapComponent>();
#pragma endregion
#pragma region W
	if (component_name == "WeaponController")
		return &add_component<WeaponController>();
#pragma endregion

	return nullptr;
}

void GameObject::remove_component_by_name(std::string component_name)
{
#pragma region A
	if (component_name == "AnimatedSpriteComponent")
		return remove_component<AnimatedSpriteComponent>();
	if (component_name == "AnimationComponent")
		return remove_component<AnimationComponent>();
	if (component_name == "AudioSource")
		return remove_component<AudioSource>();
#pragma endregion
#pragma region B
	if (component_name == "BoxColliderComponent")
		return remove_component<BoxColliderComponent>();
	if (component_name == "ButtonComponent")
		return remove_component<ButtonComponent>();
#pragma endregion
#pragma region C
	if (component_name == "CameraComponent")
		return remove_component<CameraComponent>();
	if (component_name == "CapsuleColliderComponent")
		return remove_component<CapsuleColliderComponent>();
	if (component_name == "Chest")
		return remove_component<Chest>();
	if (component_name == "ChildAnimationComponent")
		return remove_component<ChildAnimationComponent>();
	if (component_name == "CombatInventory")
		return remove_component<CombatInventory>();
#pragma endregion
#pragma region D
	if (component_name == "DontDestroyOnLoad")
		return remove_component<DontDestroyOnLoad>();
	if (component_name == "DummyAI")
		return remove_component<DummyAI>();
#pragma endregion
#pragma region I
	if (component_name == "IInteractable")
		return remove_component<IInteractable>();
	if (component_name == "Inventory")
		return remove_component<Inventory>();
	if (component_name == "InventoryGUIController")
		return remove_component<InventoryGUIController>();
	if (component_name == "InventoryWindow")
		return remove_component<InventoryWindow>();
	if (component_name == "Interactor")
		return remove_component<Interactor>();
	if (component_name == "ItemController")
		return remove_component<ItemController>();
#pragma endregion
#pragma region P
	if (component_name == "PlayerController")
		return remove_component<PlayerController>();
#pragma endregion
#pragma region R
	if (component_name == "RigidbodyComponent")
		return remove_component<RigidbodyComponent>();
#pragma endregion
#pragma region S
	if (component_name == "SceneChange")
		return remove_component<SceneChange>();
	if (component_name == "SpriteComponent")
		return remove_component<SpriteComponent>();
#pragma endregion
#pragma region T
	if (component_name == "TilemapColliderComponent")
		return remove_component<TilemapColliderComponent>();
	if (component_name == "TilemapComponent")
		return remove_component<TilemapComponent>();
#pragma endregion
#pragma region W
	if (component_name == "WeaponController")
		return remove_component<WeaponController>();
#pragma endregion
		
}

void GameObject::handle_removed_components()
{
	for (auto& c_t_r : components_to_remove)
	{
		components.erase(std::find(components.begin(), components.end(), c_t_r));
		delete c_t_r;
	}
	components_to_remove.clear();
}

}