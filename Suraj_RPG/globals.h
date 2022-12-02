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
	_LAST_OF_ENUM
};

enum class Tag
{
	UNTAGGED,
	PLAYER,
	ACTOR,
	CAMERA
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
	LAST_DONT_REMOVE
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
		std::cout << "ERROR::RENDERER::string_to_layer::UNDEFINED LAYER\n";
		return SortingLayer::BACKGROUND;
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