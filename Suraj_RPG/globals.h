#pragma once
namespace bm98
{
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
	_LAST_DONT_REMOVE
};

class Global
{

public:

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
			return "background";
		case SortingLayer::FLOOR:
			return "floor";
		case SortingLayer::BACKGROUND_DECORATIONS:
			return "background-decorations";
		case SortingLayer::ACTOR:
			return "actor";
		case SortingLayer::EFFECTS:
			return "effects";
		case SortingLayer::FOREGROUND_WALLS:
			return "foreground-walls";
		case SortingLayer::FOREGROUND:
			return "foreground";
		case SortingLayer::ITEM:
			return "item";
		case SortingLayer::UI:
			return "ui";
		default:
			std::cout << "ERROR::GLOBALS::layer_to_string::UNDEFINED LAYER\n";
			return "null";
		}
	}

	static SortingLayer string_to_layer(std::string layer)
	{
		if (layer == "background")
			return SortingLayer::BACKGROUND;
		if (layer == "floor")
			return SortingLayer::FLOOR;
		if (layer == "background-decorations")
			return SortingLayer::BACKGROUND_DECORATIONS;
		if (layer == "actor")
			return SortingLayer::ACTOR;
		if (layer == "item")
			return SortingLayer::ITEM;
		if (layer == "effects")
			return SortingLayer::EFFECTS;
		if (layer == "foreground-walls")
			return SortingLayer::FOREGROUND_WALLS;
		if (layer == "foreground")
			return SortingLayer::FOREGROUND;
		if (layer == "ui")
			return SortingLayer::UI;
		std::cout << "ERROR::GLOBAL::string_to_layer::UNDEFINED STRING\n";
		return SortingLayer::BACKGROUND;
	}

#pragma endregion

#pragma region Tags

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
			std::cout << "ERROR::GLOBALS::tag_to_string::UNDEFINED TAG\n";
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

		std::cout << "ERROR::GLOBAL::string_to_tag::UNDEFINED STRING\n";
		return Tag::UNTAGGED;
	}

#pragma endregion

#pragma region Physics_Layers

	static std::vector<std::string> physics_layer_to_vector()
	{
		return { "DEFAULT", "UI", "COLLISION", "ACTOR", "TILE", "INTERACTABLE", "DAMAGEABLE", "PLAYER", "WALLS"};
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
		default:
			std::cout << "ERROR::GLOBALS::physics_layer_to_string::UNDEFINED PHYISCS LAYER\n";
			return "null";
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

		std::cout << "ERROR::GLOBAL::string_to_physics_layer::UNDEFINED STRING\n";
		return Layer::DEFAULT;
	}

#pragma endregion



#pragma region TileType

	static std::vector<std::string> tiletypes_to_vector()
	{
		return { "Default", "Damaging" };
	}

	static TileType string_to_tiletype(std::string tile_type)
	{
		if (tile_type == "default")
			return TileType::DEFAULT;
		if (tile_type == "damaging")
			return TileType::DAMAGING;

		std::cout << "ERROR::GLOBALS::string_to_tiletype::UNDEFINED TILETYPE: " << tile_type << "\n";
		return TileType::DEFAULT;
	}

	static std::string tiletype_to_string(TileType tile_type)
	{
		switch (tile_type)
		{
		case TileType::DEFAULT:
			return "default";
		case TileType::DAMAGING:
			return "damaging";
		default:
			std::cout << "ERROR::GLOBALS::tiletype_to_string::UNDEFINED TILETYPE\n";
			return "default";
		}
	}

#pragma endregion


};
}