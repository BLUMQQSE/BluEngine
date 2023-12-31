#pragma once
#include "Math.h"
namespace bm98
{

#define PRECISION(AMOUNT) std::fixed << std::setprecision(AMOUNT)

static std::string RemoveNamespace(std::string string)
{
	int start_index = 0;

	while (string.find(":") != std::string::npos)
	{
		start_index = string.find(":");
		string = string.substr(start_index + 1);
	}

	return string;
}

class EnumFlag
{
public:
	/// <param name="all">By default false, if true will set all layers to true.</param>
	EnumFlag(std::vector<std::string> enums, bool all = false)
	{
		flags.resize(enums.size());
		for (std::size_t i = 0; i < flags.size(); i++)
		{
			flags[i] = std::make_pair(enums[i], all);
		}
	}

	int size()
	{
		return flags.size();
	}

	int count()
	{
		int result = 0;
		for (int i = 0; i < flags.size(); i++)
			if (flags[i].second)
				result++;
		return result;
	}

	template <typename T> bool contains(T in)
	{
		int index = static_cast<int>(in);
		if (index >= flags.size())
		{
			return false;
		}
		return flags[index].second;
	}

	template <typename T> bool at(T in)
	{
		int index = static_cast<int>(in);
		if (index >= flags.size())
		{
			return false;
		}
		return flags[index].second;
	}

	template <typename T> std::string string_at(T in)
	{
		int index = static_cast<int>(in);
		if (index >= flags.size())
		{
			return "";
		}
		return flags[index].first;
	}

	template <typename T> bool& operator[](T in)
	{
		int index = static_cast<int>(in);
		if (index >= flags.size())
		{
			flags.resize(index + 1);
		}
		return flags[index].second;
	}

	void operator+=(EnumFlag rhs)
	{
		for (int i = 0; i < size(); i++)
		{
			if (rhs[i] == true)
				flags[i].second = true;
		}
	}

	template <typename T> bool operator== (T rhs)
	{
		int index = static_cast<int>(rhs);
		EnumFlag flag = static_cast<EnumFlag>(rhs);
		
		if (flag.size() != size())
			return false;
		
		for (int i = 0; i < size(); i++)
		{
			if (flags[i] != flag[i])
				return false;
		}

		return true;
	}

	bool operator==(bool rhs)
	{
		for (auto f : flags)
			if (f.second)
				return rhs;
		return !rhs;
	}

	virtual Json::Value serialize_field()
	{
		Json::Value obj;
		for (int i = 0; i < flags.size(); i++)
		{
			if (flags[i].second)
				obj.append(flags[i].first);
		}
		return obj;
	}

	virtual void unserialize_field(Json::Value obj)
	{
		for (auto flag : obj)
		{
			for (auto& f : flags)
			{
				if (f.first == flag.asString())
					f.second = true;
			}
		}
	}

	std::vector<std::string> FlagToVector()
	{
		std::vector<std::string> result;
		for (int i = 0; i < flags.size(); i++)
		{
			if (flags[i].second)
				result.push_back(flags[i].first);
		}
		return result;
	}

protected:
	std::vector<std::pair<std::string, bool>> flags;

};

class Color
	: public sf::Color
{
public:
	static const sf::Color Orange;
	static const sf::Color LimeGreen;
	static const sf::Color Teal;
	static const sf::Color LightGrey;
	static const sf::Color Grey;
	static const sf::Color Charcoal;
};

namespace EffectNS
{

enum class Type
{
	STANDALONE,
	HEALTH,
	MANA,
	STAMINA,
	SPEED,
	NORMAL,
	FIRE,
	ELECTRIC
};

static std::vector<std::string> ToVector()
{
	return { "STANDALONE", "HEALTH", "MANA", "STAMINA", "SPEED", "NORMAL", "FIRE", "ELECTRIC" };
}

}

namespace Var
{
enum class Type
{
	Int,
	Float,
	Bool,
	Vector2f,
	Vector2i,
	FloatConvex,
	Dropdown,
	FlagDropdown,
	Header,
	String,
	Color
};

static std::string ToString(Type type)
{
	switch (type)
	{
	case bm98::Var::Type::Int:
		return "INT";
	case bm98::Var::Type::Float:
		return "FLOAT";
	case bm98::Var::Type::Bool:
		return "BOOL";
	case bm98::Var::Type::Vector2f:
		return "VECTOR2F";
	case bm98::Var::Type::Vector2i:
		return "VECTOR2I";
	case bm98::Var::Type::FloatConvex:
		return "FLOATCONVEX";
	case bm98::Var::Type::Dropdown:
		return "DROPDOWN";
	case bm98::Var::Type::FlagDropdown:
		return "FLAGDROPDOWN";
	case bm98::Var::Type::Header:
		return "HEADER";
	case bm98::Var::Type::String:
		return "STRING";
	case bm98::Var::Type::Color:
		return "COLOR";
	default:
		return "BOOL";
	}
}

static Type ToType(std::string type)
{
	if (type == "INT")
		return Type::Int;
	if (type == "FLOAT")
		return Type::Float;
	if (type == "BOOL")
		return Type::Bool;
	if (type == "VECTOR2F")
		return Type::Vector2f;
	if (type == "VECTOR2I")
		return Type::Vector2i;
	if (type == "FLOATCONVEX")
		return Type::FloatConvex;
	if (type == "DROPDOWN")
		return Type::Dropdown;
	if (type == "FLAGDROPDOWN")
		return Type::FlagDropdown;
	if (type == "HEADER")
		return Type::Header;
	if (type == "STRING")
		return Type::String;
	if (type == "COLOR")
		return Type::Color;

	return Type::Bool;
}

}

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

	//core::Debug::Instance()->core_log("Interaction::ToType() FAILED TO CONVERT STRING: " + type, core::Debug::LogLevel::WARNING);
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

	//core::Debug::Instance()->core_log("Interaction::ToString() FAILED TO CONVERT TYPE " + 
	//	std::to_string(static_cast<int>(type)), core::Debug::LogLevel::WARNING);
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

static std::vector<std::string> ToPriorityVector()
{
	return {"LOW", "MID", "HIGH", "MUST"};
}

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

	//core::Debug::Instance()->core_log("TileNS::ToTileType() FAILED TO CONVERT STRING: " + tile_type, core::Debug::LogLevel::WARNING);

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

		//core::Debug::Instance()->core_log("TileNS::ToString() FAILED TO CONVERT TILE TYPE " +
		//	std::to_string(static_cast<int>(tile_type)), core::Debug::LogLevel::WARNING);

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
	SHADER,
	UI,
	_LAST_DONT_REMOVE
};

/// <returns>A vector of string values for all layers in the enum.</returns>
static std::vector<std::string> ToVector()
{
	return { "BACKGROUND", "FLOOR", "BACKGROUND_DECORATIONS", "ITEM", "ACTOR", "EFFECTS",
		 "FOREGROUND_WALLS", "FOREGROUND", "SHADER", "UI"};
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
	if (layer == "SHADER")
		return Layer::SHADER;
	if (layer == "UI")
		return Layer::UI;

	//core::Debug::Instance()->core_log("Sorting::ToLayer() FAILED TO CONVERT STRING: " + layer, core::Debug::LogLevel::WARNING);
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
	case Layer::SHADER:
		return "SHADER";
	default:
		//core::Debug::Instance()->core_log("Sorting::ToString() FAILED TO CONVERT LAYER " +
		//	std::to_string(static_cast<int>(layer)), core::Debug::LogLevel::WARNING);
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

	//core::Debug::Instance()->core_log("PhysicsNS::ToLayer() FAILED TO CONVERT STRING: " + layer, core::Debug::LogLevel::WARNING);
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
		//core::Debug::Instance()->core_log("PhysicsNS::ToString() FAILED TO CONVERT LAYER " +
			//std::to_string(static_cast<int>(layer)), core::Debug::LogLevel::WARNING);
		return "DEFAULT";
	}
}

/// <summary>Bitset containing bool flags for all Physics layers.</summary>
class LayerMask : public EnumFlag
{
public:
	//std::bitset<static_cast<int>(Layer::_LAST_DONT_REMOVE)> layers;
	/// <param name="all">By default false, if true will set all layers to true.</param>
	LayerMask(bool all = false)
	: EnumFlag(ToVector(), all) 
	{
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
	TILEMAP,
	_LAST_DONT_REMOVE
};

/// <returns>A vector of string values for all tags in the enum.</returns>
static std::vector<std::string> ToVector()
{
	return { "UNTAGGED", "PLAYER", "ACTOR", "CAMERA", "INTERACTABLE", "TILEMAP"};
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
	if (tag == "TILEMAP")
		return Tag::TILEMAP;
	//core::Debug::Instance()->core_log("Tags::ToTag() FAILED TO CONVERT STRING: " + tag, core::Debug::LogLevel::WARNING);
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
	case Tag::TILEMAP:
		return "TILEMAP";
	default:

		//core::Debug::Instance()->core_log("Tags::ToString() FAILED TO CONVERT TAG " +
			//std::to_string(static_cast<int>(tag)), core::Debug::LogLevel::WARNING);
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

// returns all 8 direction starting at up and rotating clockwise
static std::vector<Vector2f> GetDirections()
{
	return {Vector2f(0, -1), Vector2f(1, -1), Vector2f(1,0), Vector2f(1, 1),
	Vector2f(0, 1), Vector2f(-1, 1), Vector2f(-1, 0), Vector2f(-1, -1)};
}


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

	//core::Debug::Instance()->core_log("Orientation::ToDirection() FAILED TO CONVERT STRING: " + direction, core::Debug::LogLevel::WARNING);
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
		//core::Debug::Instance()->core_log("Orientation::ToString() FAILED TO CONVERT TAG " +
		//	std::to_string(static_cast<int>(direction)), core::Debug::LogLevel::WARNING);
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
		//core::Debug::Instance()->core_log("ItemNS::ToString() FAILED TO CONVERT STATE " +
		//	std::to_string(static_cast<int>(state)), core::Debug::LogLevel::WARNING);
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


	//core::Debug::Instance()->core_log("ItemNS::ToState() FAILED TO CONVERT STRING: " + state, core::Debug::LogLevel::WARNING);
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
		//core::Debug::Instance()->core_log("ItemNS::ToString() FAILED TO CONVERT WEARABLE LOCATION " +
		//	std::to_string(static_cast<int>(loc)), core::Debug::LogLevel::WARNING);
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


	//core::Debug::Instance()->core_log("ItemNS::ToWearableLocation() FAILED TO CONVERT STRING: " + loc, core::Debug::LogLevel::WARNING);
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

	//core::Debug::Instance()->core_log("InventoryNS::ToType() FAILED TO CONVERT STRING: " + type, core::Debug::LogLevel::WARNING);

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
		//core::Debug::Instance()->core_log("InventoryNS::ToString() FAILED TO CONVERT TYPE " +
		//	std::to_string(static_cast<int>(type)), core::Debug::LogLevel::WARNING);
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

namespace DamageNS
{

#pragma region Type

enum class Type
{
	DEFAULT,
	FIRE, 
	ELECTRIC,
	EXPLOSIVE
};

static std::string ToString(Type type)
{
	switch (type)
	{
		case Type::DEFAULT:
			return "DEFAULT";
		case Type::FIRE:
			return "FIRE";
		case Type::ELECTRIC:
			return "ELECTRIC";
		case Type::EXPLOSIVE:
			return "EXPLOSIVE";
	}
	//core::Debug::Instance()->log("[Damager] ToString type undefined " + std::to_string((int)type),
	//							 core::Debug::LogLevel::WARNING);
	return "DEFAULT";
}

static Type ToType(std::string type)
{
	if (type == "DEFAULT")
		return Type::DEFAULT;
	if (type == "FIRE")
		return Type::FIRE;
	if (type == "ELECTRIC")
		return Type::ELECTRIC;
	if (type == "EXPLOSIVE")
		return Type::EXPLOSIVE;

	//core::Debug::Instance()->log("[Damager] ToString target undefined " + type,
	//							 core::Debug::LogLevel::WARNING);
	return Type::DEFAULT;
}

static std::vector<std::string> TypeVector()
{
	return { "DEFAULT", "FIRE", "ELECTRIC", "EXPLOSIVE"};
}

#pragma endregion



#pragma region Target

enum class Target
{
	PLAYER,
	ENEMY
};

static std::string ToString(Target target)
{
	switch (target)
	{
		case Target::PLAYER:
			return "PLAYER";
		case Target::ENEMY:
			return "ENEMY";
	}
	//core::Debug::Instance()->log("[Damager] ToString target undefined " + std::to_string((int)target),
	//							 core::Debug::LogLevel::WARNING);
	return "ENEMY";
}

static Target ToTarget(std::string target)
{
	if (target == "PLAYER")
		return Target::PLAYER;
	if (target == "ENEMY")
		return Target::ENEMY;
	//core::Debug::Instance()->log("[Damager] ToString target undefined " + target,
	//							 core::Debug::LogLevel::WARNING);
	return Target::ENEMY;
}

static std::vector<std::string> TargetVector()
{
	return { "PLAYER", "ENEMY" };
}

#pragma endregion
}

}