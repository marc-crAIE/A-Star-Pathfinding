#include "ParallelNode.h"

BehaviourStatus ParallelNode::OnUpdate(GameObject gameObject, Timestep ts)
{
	int successCount = 0;
	while (m_Current != m_Children.end())
	{
		if ((*m_Current)->Tick(gameObject, ts) == BH_SUCCESS)
			successCount++;
	}

	if (successCount >= m_SuccessCount)
		return BH_SUCCESS;
	return BH_FAILURE;
}
