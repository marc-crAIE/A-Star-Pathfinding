#include "StateMachine.h"

StateMachine::StateMachine(Ref<State> state, GameObject gameObject)
	: m_CurrentState(state)
{
	m_CurrentState->OnInit(gameObject);
}

void StateMachine::OnUpdate(GameObject gameObject, Timestep ts)
{
	if (m_CurrentState != nullptr)
	{
		bool transition = false;
		for (Ref<Transition> t : m_CurrentState->m_Transitions)
		{
			if (t->IsConditionMet(gameObject, ts))
			{
				RequestStateChange(t->m_Target);
				transition = true;
				break;
			}
		}

		if (!transition)
			m_CurrentState->OnUpdate(gameObject, ts);
	}

	if (m_NextState != nullptr)
	{
		if (m_CurrentState)
			m_CurrentState->OnExit(gameObject);
		m_NextState->OnInit(gameObject);

		m_PrevState = m_CurrentState;
		m_CurrentState = m_NextState;
		m_NextState = nullptr;
	}
}

void StateMachine::RequestStateChange(Ref<State> newState)
{
	m_NextState = newState;
}
