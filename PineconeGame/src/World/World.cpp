#include "World.h"

#include "../Utils/PerlinNoise.h"
#include "../Utils/ResourceManager.h"
#include "../Game.h"
#include "../Utils/Utils.h"

World::World()
{
	PC_PROFILE_FUNCTION();

	Generate();
}

void World::OnRender()
{
	PC_PROFILE_FUNCTION();

	float tileSize = 1.0f;
	glm::vec2 offset = { WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f };

	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			glm::vec2 tilePos = glm::vec2{ x * tileSize, y * tileSize } - offset;
			glm::vec4 color = m_Tiles[x][y]->Color;
			Ref<Texture2D> texture;
			if (!m_Tiles[x][y]->Texture.empty())
				texture = ResourceManager::GetTexture(m_Tiles[x][y]->Texture);

			if (texture && texture->IsLoaded())
				Renderer2D::DrawQuad(glm::vec3(tilePos, -0.9f), { tileSize, tileSize }, texture);
			else
				Renderer2D::DrawQuad(glm::vec3(tilePos, -0.9f), { tileSize, tileSize }, color);
		}
	}
}

Tile& World::GetTile(float x, float y) const
{
	PC_PROFILE_FUNCTION();

	glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f) - glm::vec2(-0.5f, 0.5f);
	x += offset.x;
	y += offset.y;

	if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT)
		return *Tiles::WaterTile;
	return *m_Tiles[(int)glm::floor(x)][(int)glm::floor(y)];
}

void World::Generate()
{
	PC_PROFILE_FUNCTION();

	PerlinNoise pn(7, 0.5f);

	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			float widthPercent = (float)x / WORLD_WIDTH;
			float heightPercent = (float)y / WORLD_HEIGHT;

			float noise = glm::sin(widthPercent * glm::pi<float>());
			noise *= glm::sin(heightPercent * glm::pi<float>()) + 0.05f;
			noise += pn.GetNoise(x, y) * 0.0f;

			if (noise < 0.06f)
				m_Tiles[x][y] = Tiles::WaterTile;
			else if (noise < 0.275f)
				m_Tiles[x][y] = Tiles::SandTile;
			else
				m_Tiles[x][y] = Tiles::GroundTile;

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
			}
		}
	}

	GenerateBuildings();
	GenerateDecorations();
}

void World::GenerateBuildings()
{
	PC_PROFILE_FUNCTION();

	SpawnCastle();
}

void World::GenerateDecorations()
{
	PC_PROFILE_FUNCTION();

	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			if (m_Tiles[x][y] == Tiles::GroundTile)
			{
				if (rand() % 40 == 0 && m_Tiles[x + 1][y] == Tiles::GroundTile)
				{
					SpawnTree(x, y);
				}
				else if (rand() % 5 == 0)
				{
					m_Tiles[x][y] = Tiles::GrassTile;
				}
				else if (rand() % 20 == 0)
				{
					m_Tiles[x][y] = Tiles::FlowerTile;
				}
				else if (rand() % 50 == 0)
				{
					if (rand() % 3 == 0)
						m_Tiles[x][y] = Tiles::RockTile;
					else
						m_Tiles[x][y] = Tiles::RockSmallTile;
				}
			}
		}
	}
}

void World::CreateBuildingTiles(int x, int y, int width, int height)
{
	PC_PROFILE_FUNCTION();

	int minX = (x - (width / 2));
	int maxX = (x + (width / 2));
	int minY = (y - (height / 2));
	int maxY = (y + (height / 2));

	for (int posY = minY; posY < maxY; posY++)
	{
		for (int posX = minX; posX < maxX; posX++)
		{
			m_Tiles[posX][posY] = Tiles::BuildingTile;
		}
	}
}

void World::SpawnCastle()
{
	PC_PROFILE_FUNCTION();

	int castleSize = 4;

	int posX = WORLD_WIDTH / 2;
	int posY = WORLD_HEIGHT / 2 + 1;
	CreateBuildingTiles(posX, posY - 1, castleSize, castleSize / 2);

	glm::vec3 offset = glm::vec3(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0, 0.0f) + glm::vec3(0.5f, 0.5f, 0.0f);
	float z = Utils::GetWorldZ(posY);

	auto castle = Game::GetGOmanager()->Create("Castle");

	auto& transform = castle.GetComponent<TransformComponent>();
	transform.Translation = glm::vec3(posX, posY, 0.25f) - offset;
	transform.Scale = glm::vec3(castleSize);

	auto& castleSprite = castle.AddComponent<SpriteComponent>();
	castleSprite.Texture = ResourceManager::GetTexture("assets/textures/buildings/castle.png");
}

void World::SpawnTree(int x, int y)
{
	PC_PROFILE_FUNCTION();

	m_Tiles[x][y] = Tiles::TreeTile;
	m_Tiles[x + 1][y] = Tiles::TreeTile;

	glm::vec3 offset = glm::vec3(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0, 0.0f) + glm::vec3(-0.5f, -0.5f, 0.0f);
	float z = Utils::GetWorldZ(y);

	auto tree = Game::GetGOmanager()->Create("Tree");

	auto& transform = tree.GetComponent<TransformComponent>();
	transform.Translation = glm::vec3(x, y, z) - offset;
	transform.Scale = glm::vec3(2);

	auto& treeSprite = tree.AddComponent<SpriteComponent>();
	treeSprite.Texture = ResourceManager::GetTexture("assets/textures/decorations/tree.png");
}
