#include "FallbackNode.h"

BehaviourStatus FallbackNode::OnUpdate(GameObject gameObject, Timestep ts)
{
	while (m_Current != m_Children.end())
	{
		BehaviourStatus status = (*m_Current)->Tick(gameObject, ts);

		if (status != BH_FAILURE)
		{
			if (status == BH_PENDING)
				return BH_SUCCESS;

			return status;
		}

		++m_Current;
	}
	return BH_FAILURE;
}
