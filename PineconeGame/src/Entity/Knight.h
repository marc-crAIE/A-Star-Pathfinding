#pragma once

#include "../AI/AIAgent.h"

class Knight : public AIAgent
{
public:
	void OnCreate() override;
	void OnUpdate(Timestep ts) override;

	bool IsSelected() const { return m_Selected; }
	void Select(bool selected = true) { m_Selected = selected; };

	bool IsWandering() const { return m_Wandering; }

	static float GetSpeed() { return m_MoveSpeed; }
private:
	bool m_Wandering = true;
	bool m_Selected = false;
	static float m_MoveSpeed;
};