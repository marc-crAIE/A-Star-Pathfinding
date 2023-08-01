#pragma once

#include "ControlNode.h"

class SequenceNode : public ControlNode
{
public:
	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
};