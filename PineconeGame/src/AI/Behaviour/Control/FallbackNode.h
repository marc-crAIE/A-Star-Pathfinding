#pragma once

#include "ControlNode.h"

class FallbackNode : public ControlNode
{
public:
	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
};