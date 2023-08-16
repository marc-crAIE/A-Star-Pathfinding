#include "Ship.h"

#include "../Game.h"

void Ship::OnCreate()
{
	m_Speed = 3.0f;

	auto& transform = GetComponent<TransformComponent>();

	m_Direction = -transform.Translation;
	m_Direction = glm::normalize(m_Direction);
}

void Ship::OnUpdate(Timestep ts)
{
	auto& transform = GetComponent<TransformComponent>();
	if (Game::GetWorld()->GetTile(transform.Translation.x, transform.Translation.y).Type == TILE_WATER)
	{
		transform.Translation += m_Direction * m_Speed * (float)ts;
	}
	else
	{
		Game::Get().SpawnEnemies(transform.Translation.x, transform.Translation.y);
		Game::Get().DestroyGameObject(GetGameObject());
	}
}
