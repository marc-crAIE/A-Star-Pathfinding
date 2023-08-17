#pragma once

#include "ControlNode.h"

class SequenceNode : public ControlNode
{
public:
	SequenceNode() = default;
	SequenceNode(std::initializer_list<Behaviour*> children) 
		: ControlNode(children) 
	{ }

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
};