#pragma once

#include "State.h"

class WanderState : public State
{
public:
	void OnUpdate(GameObject gameObject, Timestep ts) override;
};

class FollowState : public State
{
public:
	FollowState(GameObject target)
		: m_Target(target)
	{}

	void OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	GameObject m_Target;
};