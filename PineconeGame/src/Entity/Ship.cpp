#include "Ship.h"

#include "../Game.h"

void Ship::OnCreate()
{
	PC_PROFILE_FUNCTION();

	m_Speed = 3.0f;

	auto& transform = GetComponent<TransformComponent>();
	auto& sprite = GetComponent<SpriteComponent>();

	m_Direction = -transform.Translation;
	m_Direction = glm::normalize(m_Direction);

	if (m_Direction.x < 0.0f)
		sprite.FlipX(true);
}

void Ship::OnUpdate(Timestep ts)
{
	PC_PROFILE_FUNCTION();

	auto& transform = GetComponent<TransformComponent>();
	if (Game::GetWorld()->GetTile(transform.Translation.x, transform.Translation.y).Type == TILE_WATER)
	{
		transform.Translation += m_Direction * m_Speed * (float)ts;
	}
	else
	{
		glm::vec2 spawnPos = glm::vec2(transform.Translation);
		glm::vec2 dirToCenter = glm::normalize(glm::vec2(-transform.Translation));

		// Ensure that the enemies spawn on pathable land
		while (!Game::GetWorld()->GetTile(spawnPos.x, spawnPos.y).IsPathable())
			spawnPos += dirToCenter;

		Game::Get().SpawnEnemies(spawnPos.x, spawnPos.y - 1.0f);
		Game::Get().DestroyGameObject(GetGameObject());
	}
}
