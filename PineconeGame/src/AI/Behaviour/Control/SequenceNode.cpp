#include "SequenceNode.h"

BehaviourStatus SequenceNode::OnUpdate(GameObject gameObject, Timestep ts)
{
	while (true)
	{
		BehaviourStatus status = (*m_Current)->Tick(gameObject, ts);

		if (status != BH_SUCCESS)
			return status;

		if (++m_Current == m_Children.end())
			return BH_SUCCESS;
	}
}
