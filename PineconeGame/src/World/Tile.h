#pragma once

#include <glm/glm.hpp>
#include <Pinecone.h>

using namespace Pinecone;

enum TileType
{
	TILE_AIR,
	TILE_GROUND,
	TILE_WATER,
	TILE_WALL
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
	{}

	bool IsPathable() const { return Type == TILE_GROUND; }
public:
	TileType Type;
	glm::vec4 Color;
	std::string Texture;
};

namespace Tiles
{
	static Tile* AirTile = new Tile(TILE_AIR);
	static Tile* GroundTile = new Tile(TILE_GROUND, "assets/textures/tiles/ground.png");// glm::vec4(0.2f, 0.6f, 0.2f, 1.0f));
	static Tile* WaterTile = new Tile(TILE_WATER, glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));
	static Tile* WallTile = new Tile(TILE_WALL, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
}