#include "AttackableEntity.h"

#include "../Game.h"

void AttackableEntity::Attack(AttackableEntity* entity, int damage, Timestep ts)
{
	if (m_AttackCooldown <= 0.0f)
	{
		m_AttackCooldown = m_AttackCooldownAmount;

		entity->m_Health -= damage;
		if (entity->m_Health <= 0.0f)
			Game::GetGOmanager()->Destroy(entity->GetGameObject());

		return;
	}

	m_AttackCooldown -= ts;
}
