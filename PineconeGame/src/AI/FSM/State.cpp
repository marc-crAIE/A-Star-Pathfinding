#include "State.h"

void State::AddTransition(Ref<State> target, Transition::ConditionFn condFn)
{
	Ref<Transition> transition = CreateRef<Transition>(target, condFn);
	m_Transitions.push_back(transition);
}
