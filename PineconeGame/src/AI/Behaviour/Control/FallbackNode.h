#pragma once

#include "ControlNode.h"

class FallbackNode : public ControlNode
{
public:
	FallbackNode() = default;
	FallbackNode(std::initializer_list<Behaviour*> children)
		: ControlNode(children)
	{ }

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
};