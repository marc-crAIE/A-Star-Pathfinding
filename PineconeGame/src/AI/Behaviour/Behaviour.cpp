#include "Behaviour.h"

BehaviourStatus Behaviour::Tick(GameObject gameObject, Timestep ts)
{
	if (m_Status != BH_RUNNING && m_Status != BH_PENDING)
		OnInit();

	m_Status = OnUpdate(gameObject, ts);

	if (m_Status != BH_RUNNING)
		OnTerminate(m_Status);
	return m_Status;
}

void Behaviour::Abort()
{
	m_Status = BH_INVALID;
}

void Behaviour::Reset()
{
	OnTerminate(BH_ABORTED);
	m_Status = BH_ABORTED;
}
