#include "AIAgent.h"

AIAgent::~AIAgent()
{
	if (m_BehaviourTree)
		delete m_BehaviourTree;
}
