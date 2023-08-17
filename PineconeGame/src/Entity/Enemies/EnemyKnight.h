#pragma once

#include "../../AI/AIAgent.h"
#include "../../AI/FSM/StateMachine.h"

class EnemyKnight : public AIAgent
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

	bool IsCaptian() const { return m_Type == CAPTAIN; }
private:
	GameObject m_Castle;
	GameObject m_Leader;

	StateMachine m_State;

	Type m_Type;
};