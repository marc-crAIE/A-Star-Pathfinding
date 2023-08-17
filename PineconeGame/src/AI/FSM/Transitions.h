#pragma once

#include "States.h"

class TargetFarTransition : public Transition
{
public:
	TargetFarTransition(Ref<State> targetState, GameObject target, float distance);
private:
	GameObject m_GOTarget;
	float m_Distance;
};