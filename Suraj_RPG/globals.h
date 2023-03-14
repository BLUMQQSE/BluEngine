#pragma once
namespace bm98
{


#define PRECISION std::fixed << std::setprecision(1)

enum class TileType
{
	DEFAULT,
	DAMAGING,
	_LAST_OF_ENUM
};

enum class SortingLayer
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

enum class Tag
{
	UNTAGGED,
	PLAYER,
	ACTOR,
	CAMERA,
	_LAST_DONT_REMOVE
};

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

enum class Orientation
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Global
{
public:

	struct LayerMask
	{
		std::bitset<static_cast<int>(Layer::_LAST_DONT_REMOVE)> layers;
		LayerMask(bool all = false)
		{
			for (std::size_t i = 0; i < layers.size(); i++)
			{
				layers[i] = all;
			}
		}
		void add_layer(Layer l)
		{
			layers[static_cast<int>(l)] = true;
		}
		void add_layers(std::vector<Layer> l)
		{
			for (std::size_t i = 0; i < layers.size(); i++)
				layers[static_cast<int>(l[i])] = true;
		}
		void remove_layer(Layer l)
		{
			layers[static_cast<int>(l)] = false;
		}
		void remove_layers(std::vector<Layer> l)
		{
			for (std::size_t i = 0; i < layers.size(); i++)
				layers[static_cast<int>(l[i])] = false;
		}
	};

#pragma region SortingLayer

	static std::vector<std::string> layers_to_vector()
	{
		return { "BACKGROUND", "FLOOR", "BACKGROUND_DECORATIONS", "ITEM", "ACTOR", "EFFECTS",
			 "FOREGROUND_WALLS", "FOREGROUND", "UI" };
	}

	static std::string layer_to_string(SortingLayer layer)
	{
		switch (layer)
		{
		case SortingLayer::BACKGROUND:
			return "BACKGROUND";
		case SortingLayer::FLOOR:
			return "FLOOR";
		case SortingLayer::BACKGROUND_DECORATIONS:
			return "BACKGROUND_DECORATIONS";
		case SortingLayer::ACTOR:
			return "ACTOR";
		case SortingLayer::EFFECTS:
			return "EFFECTS";
		case SortingLayer::FOREGROUND_WALLS:
			return "FOREGROUND_WALLS";
		case SortingLayer::FOREGROUND:
			return "FOREGROUND";
		case SortingLayer::ITEM:
			return "ITEM";
		case SortingLayer::UI:
			return "UI";
		default:
			std::cout << "ERROR::GLOBALS::layer_to_string::UNDEFINED LAYER: " << static_cast<int>(layer) << "\n";
			return "null";
		}
	}

	static SortingLayer string_to_layer(std::string layer)
	{
		if (layer == "BACKGROUND")
			return SortingLayer::BACKGROUND;
		if (layer == "FLOOR")
			return SortingLayer::FLOOR;
		if (layer == "BACKGROUND_DECORATIONS")
			return SortingLayer::BACKGROUND_DECORATIONS;
		if (layer == "ACTOR")
			return SortingLayer::ACTOR;
		if (layer == "ITEM")
			return SortingLayer::ITEM;
		if (layer == "EFFECTS")
			return SortingLayer::EFFECTS;
		if (layer == "FOREGROUND_WALLS")
			return SortingLayer::FOREGROUND_WALLS;
		if (layer == "FOREGROUND")
			return SortingLayer::FOREGROUND;
		if (layer == "UI")
			return SortingLayer::UI;
		std::cout << "ERROR::GLOBAL::string_to_layer::UNDEFINED STRING: " << layer << "\n";
		return SortingLayer::BACKGROUND;
	}

#pragma endregion


#pragma region Tag

	static std::vector<std::string> tags_to_vector()
	{
		return { "UNTAGGED", "PLAYER", "ACTOR", "CAMERA" };
	}

	static std::string tag_to_string(Tag tag)
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
		default:
			std::cout << "ERROR::GLOBALS::tag_to_string::UNDEFINED TAG: " << static_cast<int>(tag) << "\n";
			return "null";
		}
	}

	static Tag string_to_tag(std::string tag)
	{
		if (tag == "UNTAGGED")
			return Tag::UNTAGGED;
		if (tag == "PLAYER")
			return Tag::PLAYER;
		if (tag == "ACTOR")
			return Tag::ACTOR;
		if (tag == "CAMERA")
			return Tag::CAMERA;

		std::cout << "ERROR::GLOBAL::string_to_tag::UNDEFINED STRING: "<< tag << "\n";
		return Tag::UNTAGGED;
	}

#pragma endregion


#pragma region Physics_Layer

	static std::vector<std::string> physics_layer_to_vector()
	{
		return { "DEFAULT", "UI", "COLLISION", "ACTOR", "TILE", 
			"INTERACTABLE", "DAMAGEABLE", "PLAYER", "WALLS", "PHYSICS_IGNORE"};
	}

	static std::string physics_layer_to_string(Layer layer)
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
			std::cout << "ERROR::GLOBALS::physics_layer_to_string::UNDEFINED PHYISCS LAYER: " << static_cast<int>(layer) << "\n";
			return "DEFAULT";
		}
	}

	static Layer string_to_physics_layer(std::string layer)
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

		std::cout << "ERROR::GLOBAL::string_to_physics_layer::UNDEFINED STRING: " << layer << "\n";
		return Layer::DEFAULT;
	}

#pragma endregion


#pragma region TileType

	static std::vector<std::string> tiletypes_to_vector()
	{
		return { "DEFAULT", "DAMAGING" };
	}

	static TileType string_to_tiletype(std::string tile_type)
	{
		if (tile_type == "DEFAULT")
			return TileType::DEFAULT;
		if (tile_type == "DAMAGING")
			return TileType::DAMAGING;

		std::cout << "ERROR::GLOBALS::string_to_tiletype::UNDEFINED TILETYPE: " << tile_type << "\n";
		return TileType::DEFAULT;
	}

	static std::string tiletype_to_string(TileType tile_type)
	{
		switch (tile_type)
		{
		case TileType::DEFAULT:
			return "DEFAULT";
		case TileType::DAMAGING:
			return "DAMAGING";
		default:
			std::cout << "ERROR::GLOBALS::tiletype_to_string::UNDEFINED TILETYPE: " << static_cast<int>(tile_type) << "\n";
			return "DEFAULT";
		}
	}

#pragma endregion


#pragma region Oritentation

	static std::string orientation_to_string(Orientation orientation)
	{
		switch (orientation)
		{
		case Orientation::DOWN:
			return "DOWN";
		case Orientation::UP:
			return "UP";
		case Orientation::LEFT:
			return "LEFT";
		case Orientation::RIGHT:
			return "RIGHT";
		default:
			std::cout << "ERROR::GLOBALS::orientation_to_string::UNDEFINED LAYER: " << 
				static_cast<int>(orientation) << "\n";
			return "DOWN";
		}
	}

	static Orientation string_to_orientation(std::string orientation)
	{
		if (orientation == "DOWN")
			return Orientation::DOWN;
		if (orientation == "UP")
			return Orientation::UP;
		if (orientation == "LEFT")
			return Orientation::LEFT;
		if (orientation == "RIGHT")
			return Orientation::RIGHT;

		std::cout << "ERROR::GLOBAL::string_to_orientation::UNDEFINED STRING: " << orientation << "\n";
		return Orientation::DOWN;
	}

#pragma endregion


};
}