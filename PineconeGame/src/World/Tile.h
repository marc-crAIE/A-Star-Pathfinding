#pragma once

#include <glm/glm.hpp>
#include <Pinecone.h>

using namespace Pinecone;

enum TileType
{
	TILE_AIR,
	TILE_GROUND,
	TILE_WATER,
	TILE_TREE,
	TILE_ROCK,
	TILE_BUILDING
};

struct Tile
{
public:
	Tile(TileType type)
		: Type(type)
	{
		Color = glm::vec4(0.0f);
	}

	Tile(TileType type, glm::vec4& color)
		: Type(type), Color(color)
	{}

	Tile(TileType type, const std::string& texture)
		: Type(type), Texture(texture)
	{
		Color = glm::vec4(0.0f);
	}

	bool IsPathable() const { return Type == TILE_GROUND; }
public:
	TileType Type;
	glm::vec4 Color;
	std::string Texture;
};

namespace Tiles
{
	static Tile* AirTile = new Tile(TILE_AIR);
	static Tile* GroundTile = new Tile(TILE_GROUND, "assets/textures/tiles/ground.png");
	static Tile* GrassTile = new Tile(TILE_GROUND, "assets/textures/tiles/grass.png");
	static Tile* SandTile = new Tile(TILE_GROUND, "assets/textures/tiles/sand.png");
	static Tile* FlowerTile = new Tile(TILE_GROUND, "assets/textures/tiles/flower.png");
	static Tile* WaterTile = new Tile(TILE_WATER, glm::vec4(0.165f, 0.49f, 0.459f, 1.0f));
	static Tile* TreeTile = new Tile(TILE_TREE, "assets/textures/tiles/ground.png");
	static Tile* RockTile = new Tile(TILE_ROCK, "assets/textures/tiles/rock.png");
	static Tile* RockSmallTile = new Tile(TILE_ROCK, "assets/textures/tiles/rock_small.png");
	static Tile* BuildingTile = new Tile(TILE_BUILDING, "assets/textures/tiles/ground.png");
}