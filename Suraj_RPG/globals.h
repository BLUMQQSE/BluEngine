#pragma once
#include "Math.h"
namespace bm98
{

#define PRECISION std::fixed << std::setprecision(1)

/// <summary> Namespace containing Interaction enums and conversion methods. </summary>
namespace Interaction
{

enum class Type
{
	INSTANT,
	DURATION,
	/// <summary>Continue until some input is given to end the interaction.</summary>
	INPUT,
	_LAST_DONT_REMOVE
};

static std::vector<std::string> ToVector()
{
	return { "INSTANT","DURATION","INPUT" };
}

static Type ToType(std::string type)
{
	if (type == "INSTANT")
		return Type::INSTANT;
	if (type == "DURATION")
		return Type::DURATION;
	if (type == "INPUT")
		return Type::INPUT;

	std::cout << "ERROR::INTERACTION::ToType::FAILED TO CONVERT STRING " << type << "\n";
	return Type::INSTANT;
}

static std::string ToString(Type type)
{
	switch (type)
	{
	case Type::INSTANT:
		return "INSTANT";
	case Type::DURATION:
		return "DURATION";
	case Type::INPUT:
		return "INPUT";
	}
	std::cout << "ERROR::INTERACTION::ToString::FAILED TO CONVERT TYPE " << static_cast<int>(type)<<"\n";
	return "INSTANT";
}

/// <summary>Priority of an interaction.</summary>
enum class Priority
{
	/// <summary>If current interaction exists will be ignored.</summary>
	LOW,
	/// <summary>Takes priority of LOW current interaction if exists.</summary>
	MID,
	/// <summary>Takes priority of LOW and MID current interaction if exists.</summary>
	HIGH,
	/// <summary>Will immediately begin and end current interaction if exists.</summary>
	MUST,
	_LAST_DONT_REMOVE
};

/// <returns>A Priority corresponding to the string value provided.</returns>
static Priority ToPriority(std::string s)
{
	if (s == "MUST")
		return Priority::MUST;
	if (s == "HIGH")
		return Priority::HIGH;
	if (s == "MID")
		return Priority::MID;

	return Priority::LOW;
}

/// <returns>A string value corresponding to the Priority provided.</returns>
static std::string ToString(Priority p)
{
	switch (p)
	{

	case Priority::MUST:
		return "MUST";
	case Priority::HIGH:
		return "HIGH";
	case Priority::MID:
		return "MID";
	default:
		return "LOW";
	}
}

}

/// <summary> Namespace containing Tile enums and conversion methods. </summary>
namespace TileNS
{
/// <summary>Type of tile.</summary>
enum class Type
{
	DEFAULT,
	DAMAGING,
	_LAST_DONT_REMOVE
};

/// <returns>A vector of string values for all tile types in the enum.</returns>
static std::vector<std::string> ToVector()
{
	return { "DEFAULT", "DAMAGING" };
}

/// <returns>A Type corresponding to the string value provided.</returns>
static Type ToTiletype(std::string tile_type)
{
	if (tile_type == "DEFAULT")
		return Type::DEFAULT;
	if (tile_type == "DAMAGING")
		return Type::DAMAGING;

	std::cout << "ERROR::TileNS::ToType::UNDEFINED TILETYPE: " << tile_type << "\n";
	return Type::DEFAULT;
}

/// <returns>A string value corresponding to the Type provided.</returns>
static std::string ToString(Type tile_type)
{
	switch (tile_type)
	{
	case Type::DEFAULT:
		return "DEFAULT";
	case Type::DAMAGING:
		return "DAMAGING";
	default:
		std::cout << "ERROR::TileNS::ToString::UNDEFINED TILETYPE: " << static_cast<int>(tile_type) << "\n";
		return "DEFAULT";
	}
}

}

/// <summary> Namespace containing Sorting enums and conversion methods. </summary>
namespace Sorting
{
/// <summary>Layer which a renderable object should be rendered.</summary>
enum class Layer
{
	BACKGROUND,
	FLOOR,
	BACKGROUND_DECORATIONS,
	ITEM,
	ACTOR,
	EFFECTS,
	FOREGROUND_WALLS,
	FOREGROUND,
	UI,
	_LAST_DONT_REMOVE
};

/// <returns>A vector of string values for all layers in the enum.</returns>
static std::vector<std::string> ToVector()
{
	return { "BACKGROUND", "FLOOR", "BACKGROUND_DECORATIONS", "ITEM", "ACTOR", "EFFECTS",
		 "FOREGROUND_WALLS", "FOREGROUND", "UI" };
}

/// <returns>A Layer corresponding to the string value provided.</returns>
static Layer ToLayer(std::string layer)
{
	if (layer == "BACKGROUND")
		return Layer::BACKGROUND;
	if (layer == "FLOOR")
		return Layer::FLOOR;
	if (layer == "BACKGROUND_DECORATIONS")
		return Layer::BACKGROUND_DECORATIONS;
	if (layer == "ACTOR")
		return Layer::ACTOR;
	if (layer == "ITEM")
		return Layer::ITEM;
	if (layer == "EFFECTS")
		return Layer::EFFECTS;
	if (layer == "FOREGROUND_WALLS")
		return Layer::FOREGROUND_WALLS;
	if (layer == "FOREGROUND")
		return Layer::FOREGROUND;
	if (layer == "UI")
		return Layer::UI;
	std::cout << "ERROR::Sorting::ToLayer::UNDEFINED STRING: " << layer << "\n";
	return Layer::BACKGROUND;
}

/// <returns>A string value corresponding to the Layer provided.</returns>
static std::string ToString(Layer layer)
{
	switch (layer)
	{
	case Layer::BACKGROUND:
		return "BACKGROUND";
	case Layer::FLOOR:
		return "FLOOR";
	case Layer::BACKGROUND_DECORATIONS:
		return "BACKGROUND_DECORATIONS";
	case Layer::ACTOR:
		return "ACTOR";
	case Layer::EFFECTS:
		return "EFFECTS";
	case Layer::FOREGROUND_WALLS:
		return "FOREGROUND_WALLS";
	case Layer::FOREGROUND:
		return "FOREGROUND";
	case Layer::ITEM:
		return "ITEM";
	case Layer::UI:
		return "UI";
	default:
		std::cout << "ERROR::Sorting::ToString::UNDEFINED LAYER: " << static_cast<int>(layer) << "\n";
		return "null";
	}
}


}

/// <summary> Namespace containing physics related enums and conversion methods. </summary>
namespace PhysicsNS
{
/// <summary>Layer which a gameobject should exists in the physics engine.</summary>
enum class Layer
{
	DEFAULT,
	UI,
	COLLISION,
	ACTOR,
	TILE,
	INTERACTABLE,
	DAMAGEABLE,
	PLAYER,
	WALLS,
	PHYSICS_IGNORE,
	_LAST_DONT_REMOVE
};

/// <returns>A vector of string values for all layers in the enum.</returns>
static std::vector<std::string> ToVector()
{
	return { "DEFAULT", "UI", "COLLISION", "ACTOR", "TILE",
		"INTERACTABLE", "DAMAGEABLE", "PLAYER", "WALLS", "PHYSICS_IGNORE" };
}

/// <returns>A Layer corresponding to the string value provided.</returns>
static Layer ToLayer(std::string layer)
{
	if (layer == "DEFAULT")
		return Layer::DEFAULT;
	if (layer == "UI")
		return Layer::UI;
	if (layer == "COLLISION")
		return Layer::COLLISION;
	if (layer == "ACTOR")
		return Layer::ACTOR;
	if (layer == "TILE")
		return Layer::TILE;
	if (layer == "INTERACTABLE")
		return Layer::INTERACTABLE;
	if (layer == "DAMAGEABLE")
		return Layer::DAMAGEABLE;
	if (layer == "PLAYER")
		return Layer::PLAYER;
	if (layer == "WALLS")
		return Layer::WALLS;
	if (layer == "PHYSICS_IGNORE")
		return Layer::PHYSICS_IGNORE;

	std::cout << "ERROR::PhysicsNS::ToLayer::UNDEFINED STRING: " << layer << "\n";
	return Layer::DEFAULT;
}

/// <returns>A string value corresponding to the Layer provided.</returns>
static std::string ToString(Layer layer)
{
	switch (layer)
	{
	case Layer::DEFAULT:
		return "DEFAULT";
	case Layer::UI:
		return "UI";
	case Layer::COLLISION:
		return "COLLISION";
	case Layer::ACTOR:
		return "ACTOR";
	case Layer::TILE:
		return "TILE";
	case Layer::INTERACTABLE:
		return "INTERACTABLE";
	case Layer::DAMAGEABLE:
		return "DAMAGEABLE";
	case Layer::PLAYER:
		return "PLAYER";
	case Layer::WALLS:
		return "WALLS";
	case Layer::PHYSICS_IGNORE:
		return "PHYSICS_IGNORE";
	default:
		std::cout << "ERROR::PhysicsNS::ToString::UNDEFINED PHYISCS LAYER: " << static_cast<int>(layer) << "\n";
		return "DEFAULT";
	}
}

/// <summary>Bitset containing bool flags for all Physics layers.</summary>
struct LayerMask
{
	std::bitset<static_cast<int>(Layer::_LAST_DONT_REMOVE)> layers;
	/// <param name="all">By default false, if true will set all layers to true.</param>
	LayerMask(bool all = false)
	{
		for (std::size_t i = 0; i < layers.size(); i++)
		{
			layers[i] = all;
		}
	}
	/// <param name="l">Sets bool flag for Layer l to true.</param>
	void add_layer(Layer l)
	{
		layers[static_cast<int>(l)] = true;
	}
	/// <param name="l">Sets bool flag for Layers l to true.</param>
	void add_layers(std::vector<Layer> l)
	{
		for (std::size_t i = 0; i < layers.size(); i++)
			layers[static_cast<int>(l[i])] = true;
	}
	/// <param name="l">Sets bool flag for Layer l to false.</param>
	void remove_layer(Layer l)
	{
		layers[static_cast<int>(l)] = false;
	}
	/// <param name="l">Sets bool flag for Layers l to false.</param>
	void remove_layers(std::vector<Layer> l)
	{
		for (std::size_t i = 0; i < layers.size(); i++)
			layers[static_cast<int>(l[i])] = false;
	}

	/// <returns>A vector of string values and their respective bool flags for all layers of the LayerMask 
	/// layers.</returns>
	static std::vector<std::pair<std::string, bool>> ToVector(std::bitset<static_cast<int>(Layer::_LAST_DONT_REMOVE)> layers)
	{
		std::vector<std::pair<std::string, bool>> vec;
		for (int i = 0; i < static_cast<int>(Layer::_LAST_DONT_REMOVE); i++)
		{
			vec.push_back(std::make_pair(ToString(static_cast<Layer>(i)), layers[i]));
		}
		return vec;
	}

};

}

enum class EditorPanel
{
	ALL,
	HEIRARCHY,
	CONTEXT,
	INSPECTOR,
	_LAST_DONT_REMOVE
};

/// <summary> Namespace containing tag enum and conversion methods. </summary>
namespace Tags
{
/// <summary>Tag identifier for gameobjects.</summary>
enum class Tag
{
	UNTAGGED,
	PLAYER,
	ACTOR,
	CAMERA,
	INTERACTABLE,
	_LAST_DONT_REMOVE
};

/// <returns>A vector of string values for all tags in the enum.</returns>
static std::vector<std::string> ToVector()
{
	return { "UNTAGGED", "PLAYER", "ACTOR", "CAMERA", "INTERACTABLE" };
}

/// <returns>A Tag corresponding to the string value provided.</returns>
static Tag ToTag(std::string tag)
{
	if (tag == "UNTAGGED")
		return Tag::UNTAGGED;
	if (tag == "PLAYER")
		return Tag::PLAYER;
	if (tag == "ACTOR")
		return Tag::ACTOR;
	if (tag == "CAMERA")
		return Tag::CAMERA;
	if (tag == "INTERACTABLE")
		return Tag::INTERACTABLE;

	std::cout << "ERROR::Tags::ToTag::UNDEFINED STRING: " << tag << "\n";
	return Tag::UNTAGGED;
}

/// <returns>A string value corresponding to the Tag provided.</returns>
static std::string ToString(Tag tag)
{
	switch (tag)
	{
	case Tag::UNTAGGED:
		return "UNTAGGED";
	case Tag::PLAYER:
		return "PLAYER";
	case Tag::ACTOR:
		return "ACTOR";
	case Tag::CAMERA:
		return "CAMERA";
	case Tag::INTERACTABLE:
		return "INTERACTABLE";
	default:
		std::cout << "ERROR::Tags::ToString::UNDEFINED TAG: " << static_cast<int>(tag) << "\n";
		return "null";
	}
}


}

/// <summary> Namespace containing orientation enums and conversion methods. </summary>
namespace Orientation
{
/// <summary>Direction of orientation.</summary>
enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	_LAST_DONT_REMOVE
};

/// <returns>A Direction corresponding to the string value provided.</returns>
static Direction ToDirection(std::string direction)
{
	if (direction == "DOWN")
		return Direction::DOWN;
	if (direction == "UP")
		return Direction::UP;
	if (direction == "LEFT")
		return Direction::LEFT;
	if (direction == "RIGHT")
		return Direction::RIGHT;

	std::cout << "ERROR::Orientation::ToDirection::UNDEFINED STRING: " << direction << "\n";
	return Direction::DOWN;
}

/// <returns>A string value corresponding to the Direction provided.</returns>
static std::string ToString(Direction direction)
{
	switch (direction)
	{
	case Direction::DOWN:
		return "DOWN";
	case Direction::UP:
		return "UP";
	case Direction::LEFT:
		return "LEFT";
	case Direction::RIGHT:
		return "RIGHT";
	default:
		std::cout << "ERROR::Orientation::ToString::UNDEFINED Direction: " <<
			static_cast<int>(direction) << "\n";
		return "DOWN";
	}
}

static Direction VectorToDirection(Vector2f vec)
{
	if (std::abs(vec.x) > std::abs(vec.y))
	{
		// left or right
		if (vec.x < 0)
			return Direction::LEFT;
		
		return Direction::RIGHT;
	}
	else
	{
		//up or down
		if (vec.y < 0)
			return Direction::UP;
		
		return Direction::DOWN;

	}

}


}

namespace ItemNS
{

enum class State
{
	DROPPED,
	IN_USE,
	_LAST_DONT_REMOVE
};

enum class WearableLocation
{
	HELMET,
	CHEST,
	SHINS,
	BRACELET,
	NECKLACE,
	RING,
	RIGHT,
	LEFT,
	MAGIC_SPELL,
	NONE,
	_LAST_DONT_REMOVE
};

static std::vector<std::string> ToVector()
{
	return {"DROPPED", "IN_USE"};
}

static std::string ToString(State state)
{
	switch (state)
	{
	case bm98::ItemNS::State::DROPPED:
		return "DROPPED";
	case bm98::ItemNS::State::IN_USE:
		return "IN_USE";
	default:
		std::cout << "ERROR::ItemNS::ToString::UNDEFINED String: " << static_cast<int>(state) << "\n";
		return "DROPPED";

		break;
	}
}

static State ToState(std::string state)
{
	if (state == "DROPPED")
		return State::DROPPED;
	if (state == "IN_USE")
		return State::IN_USE;

	std::cout << "ERROR::ItemNS::ToState::UNDEFINED State: " << state << "\n";
}

static std::vector<std::string> WearableToVector()
{
	return { "HELMET", "CHEST", "SHINS", "BRACELET", "NECKLACE", "RING", "RIGHT", "LEFT", "MAGIC_SPELL", "NONE"};
}

static std::string ToString(WearableLocation loc)
{
	switch (loc)
	{
	case bm98::ItemNS::WearableLocation::NONE:
		return "NONE";
	case bm98::ItemNS::WearableLocation::HELMET:
		return "HELMET";
	case bm98::ItemNS::WearableLocation::CHEST:
		return "CHEST";
	case bm98::ItemNS::WearableLocation::SHINS:
		return "SHINS";
	case bm98::ItemNS::WearableLocation::BRACELET:
		return "BRACELET";
	case bm98::ItemNS::WearableLocation::NECKLACE:
		return "NECKLACE";
	case bm98::ItemNS::WearableLocation::RING:
		return "RING";
	case bm98::ItemNS::WearableLocation::RIGHT:
		return "RIGHT";
	case bm98::ItemNS::WearableLocation::LEFT:
		return "LEFT";
	case bm98::ItemNS::WearableLocation::MAGIC_SPELL:
		return "MAGIC_SPELL";
	default:
		return "NONE";
	}
}

static WearableLocation ToWearableLocation(std::string loc)
{
	if (loc == "NONE")
		return WearableLocation::NONE;
	if (loc == "HELMET")
		return WearableLocation::HELMET;
	if (loc == "CHEST")
		return WearableLocation::CHEST;
	if (loc == "SHINS")
		return WearableLocation::SHINS;
	if (loc == "BRACELET")
		return WearableLocation::BRACELET;
	if (loc == "NECKLACE")
		return WearableLocation::NECKLACE;
	if (loc == "RING")
		return WearableLocation::RING;
	if (loc == "RIGHT")
		return WearableLocation::RIGHT;
	if (loc == "LEFT")
		return WearableLocation::LEFT;
	if (loc == "MAGIC_SPELL")
		return WearableLocation::MAGIC_SPELL;

	return WearableLocation::NONE;

}

}


namespace InventoryNS
{

enum class Type
{
	GENERAL,
	COMBAT,
	SHOP
};

static std::vector<std::string> ToVector()
{
	return { "GENERAL", "COMBAT", "SHOP" };
}

static Type ToType(std::string type)
{
	if (type == "GENERAL")
		return Type::GENERAL;
	if (type == "COMBAT")
		return Type::COMBAT;
	if (type == "SHOP")
		return Type::SHOP;

	std::cout << "ERROR::INVENTORY::ToString::UNDEFINED TYPE " << type << "\n";

}

static std::string ToString(Type type)
{
	switch (type)
	{
	case bm98::InventoryNS::Type::GENERAL:
		return "GENERAL";
	case bm98::InventoryNS::Type::COMBAT:
		return "COMBAT";
	case bm98::InventoryNS::Type::SHOP:
		return "SHOP";
	default:

		std::cout << "ERROR::INVENTORY::ToString::UNDEFINED TYPE " <<
			static_cast<int>(type) << "\n";

		return "GENERAL";
		break;
	}
}

enum class WindowToggle
{
	GENERAL_INVENTORY,
	COMBAT_INVENTORY,
	/// <summary>General inventory AND combat inventory.</summary>
	FULL_INVENTORY,
	EXTERNAL_INVENTORY,
	CLOSE_ALL,
	/// <summary>Open all including external if available</summary>
	OPEN_ALL,
	_LAST_DONT_REMOVE
};

}
}