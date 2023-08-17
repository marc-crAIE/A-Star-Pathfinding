#pragma once

#include <Pinecone.h>

using namespace Pinecone;

class StateMachine;
class State;

class Transition
{
public:
	using ConditionFn = std::function<bool(GameObject, Timestep)>;
public:
	Transition(Ref<State> target)
		: m_Target(target), m_ConditionFn([](GameObject go, Timestep ts) -> bool { return false; })
	{}

	Transition(Ref<State> target, ConditionFn condFn)
		: m_Target(target), m_ConditionFn(std::move(condFn))
	{}
protected:
	bool IsConditionMet(GameObject gameObject, Timestep ts) { return m_ConditionFn(gameObject, ts); }
protected:
	Ref<State> m_Target;
	ConditionFn m_ConditionFn;
private:
	friend StateMachine;
};

class State
{
public:
	virtual void OnInit(GameObject gameObject) {}
	virtual void OnExit(GameObject gameObject) {}
	virtual void OnUpdate(GameObject gameObject, Timestep ts) = 0;

	void AddTransition(Ref<Transition> transition) { m_Transitions.push_back(transition); }
	void AddTransition(Ref<State> target, Transition::ConditionFn condFn);
private:
	std::vector<Ref<Transition>> m_Transitions;
private:
	friend StateMachine;
};