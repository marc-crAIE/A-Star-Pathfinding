#pragma once

#include "AttackableEntity.h"

class Knight : public AttackableEntity
{
public:
	void OnCreate() override;
	void OnUpdate(Timestep ts) override;

	bool IsSelected() const { return m_Selected; }
	void Select(bool selected = true) { m_Selected = selected; };

	bool IsAttacking() const { return m_AttackingTarget; }
	void SetAttackTarget(GameObject target) { m_AttackingTarget = true; m_AttackTarget = target; }
	void ResetAttackTarget() { m_AttackingTarget = false; }
	GameObject GetAttackTarget() { return m_AttackTarget; }

	bool IsWandering() const { return m_Wandering; }
private:
	GameObject m_Castle;
	GameObject m_AttackTarget;

	float m_EnemyDetectionDistance = 6.0f;
	float m_EnemyEscapeOffset = 2.0f;

	bool m_Wandering = true;
	bool m_Selected = false;
	bool m_AttackingTarget = false;
};