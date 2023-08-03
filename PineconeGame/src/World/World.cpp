#include "World.h"

World::World()
{
	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			if ((x == 0 || x == WORLD_WIDTH - 1) || (y == 0 || y == WORLD_HEIGHT - 1))
			{
				m_Tiles[x][y] = Tiles::WaterTile;
			}
			else
			{
				if (x >= (WORLD_WIDTH / 2 - 2) && x <= (WORLD_WIDTH / 2 + 2) &&
					y >= (WORLD_HEIGHT / 2 - 2) && y <= (WORLD_HEIGHT / 2 + 2))
				{
					m_Tiles[x][y] = Tiles::GroundTile;
				}
				else
					m_Tiles[x][y] = (rand() % 3 == 0) ? Tiles::WallTile : Tiles::GroundTile;
			}
		}
	}
}

void World::OnRender()
{
	float tileSize = 1.0f;
	glm::vec2 offset = { WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f };

	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			glm::vec2 tilePos = glm::vec2{ x * tileSize, y * tileSize } - offset;
			glm::vec4 color = m_Tiles[x][y]->GetColor();
			Renderer2D::DrawQuad(glm::vec3(tilePos, -0.9f), { tileSize, tileSize }, color);
		}
	}
}
