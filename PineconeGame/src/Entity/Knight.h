#pragma once

#include "../AI/AIAgent.h"

class Knight : public AIAgent
{
public:
	void OnCreate() override;

	void OnUpdate(Timestep ts) override;
private:
	bool m_Selected = false;
};