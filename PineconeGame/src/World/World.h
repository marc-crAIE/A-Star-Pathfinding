#pragma once

#include <Pinecone.h>

#include "Tile.h"

using namespace Pinecone;

#define WORLD_WIDTH		50
#define WORLD_HEIGHT	30

class World
{
public:
	World();
	~World() = default;

	void OnRender();

	Tile& GetTile(int x, int y) const { return *m_Tiles[x][y]; }
private:
	Tile* m_Tiles[WORLD_WIDTH][WORLD_HEIGHT];
};