#pragma once

#include "../../AI/AIAgent.h"

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

	bool IsCaptian() const { return m_Type == CAPTAIN; }
private:
	GameObject m_Castle;

	Type m_Type;
};