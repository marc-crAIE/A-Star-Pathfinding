#pragma once

#include <glm/glm.hpp>

enum TileType
{
	TILE_AIR,
	TILE_GROUND,
	TILE_WATER,
	TILE_WALL
};

class Tile
{
public:
	Tile(TileType type)
		: m_Type(type)
	{
		m_Color = glm::vec4(0.0f);
	}

	Tile(TileType type, glm::vec4& color)
		: m_Type(type), m_Color(color)
	{}

	TileType GetType() const { return m_Type; }
	glm::vec4 GetColor() const { return m_Color; }
	bool IsPathable() const { return m_Type == TILE_GROUND; }
protected:
	TileType m_Type;
	glm::vec4 m_Color;
};

namespace Tiles
{
	static Tile* AirTile = new Tile(TILE_AIR);
	static Tile* GroundTile = new Tile(TILE_GROUND, glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
	static Tile* WaterTile = new Tile(TILE_WATER, glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));
	static Tile* WallTile = new Tile(TILE_WALL, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
}