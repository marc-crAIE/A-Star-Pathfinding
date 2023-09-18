#pragma once

#include "../AttackableEntity.h"
#include "../../AI/FSM/StateMachine.h"

class EnemyKnight : public AttackableEntity
{
public:
	enum Type
	{
		KNIGHT,
		CAPTAIN
	};
public:
	EnemyKnight(bool captain = false);

	void OnCreate() override;
	void OnUpdate(Timestep ts) override;

	void SetLeader(GameObject leader) { m_Leader = leader; }

	GameObject GetAttackTarget() const { return m_AttackingTarget; }
	void SetAttackTarget(GameObject attackTarget) { m_AttackingTarget = attackTarget; }

	bool IsCaptian() const { return m_Type == CAPTAIN; }
private:
	GameObject m_Castle;
	GameObject m_Leader;
	GameObject m_AttackingTarget;

	StateMachine m_State;

	Type m_Type;
};