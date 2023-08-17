#pragma once

#include "ControlNode.h"

class ParallelNode : public ControlNode
{
public:
	ParallelNode(int successCount = 1) : m_SuccessCount(successCount) {}
	ParallelNode(int successCount, std::initializer_list<Behaviour*> children)
		: ControlNode(children)
	{
		m_SuccessCount = successCount;
	}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	int m_SuccessCount;
};