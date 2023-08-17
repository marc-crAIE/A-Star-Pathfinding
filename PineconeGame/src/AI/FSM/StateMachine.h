#pragma once

#include "States.h"
#include "Transitions.h"

class StateMachine
{
public:
	StateMachine() = default;
	StateMachine(Ref<State> state, GameObject gameObject);

	void OnUpdate(GameObject gameObject, Timestep ts);
	void RequestStateChange(Ref<State> newState);
private:
	Ref<State> m_CurrentState;
	Ref<State> m_NextState;
	Ref<State> m_PrevState;
};