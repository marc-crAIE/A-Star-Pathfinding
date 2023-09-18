#pragma once

#include "../AI/AIAgent.h"

class AttackableEntity : public AIAgent
{
public:
	virtual void Attack(AttackableEntity* entity, int damage, Timestep ts);

	float GetHealth() const { return m_Health; }
	float GetDamageAmount() const { return m_DamageAmount; }
	float GetAttackRange() const { return m_AttackRange; }
protected:
	float m_Health = 30.0f;

	float m_AttackCooldown = 0.0f;
	float m_AttackCooldownAmount = 1.0f;

	float m_DamageAmount = 10.0f;
	float m_AttackRange = 1.5f;
};