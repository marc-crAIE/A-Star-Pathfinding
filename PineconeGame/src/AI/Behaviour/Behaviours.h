#pragma once

#include <functional>

#include <Pinecone.h>

using namespace Pinecone;

#include "Behaviour.h"
#include "Control/ControlNode.h"
#include "Control/SequenceNode.h"
#include "Control/FallbackNode.h"
#include "Control/ParallelNode.h"

#pragma region General

class FuncAction : public Behaviour
{
public:
	using ActionFn = std::function<BehaviourStatus()>;
public:
	FuncAction(ActionFn func) : m_Func(std::move(func)) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts);
private:
	ActionFn m_Func;
};

#pragma endregion

#pragma region Mouse

class MouseNearCondition : public Behaviour
{
public:
	MouseNearCondition(float distance) : m_MouseDistance(distance) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts);
private:
	float m_MouseDistance;
};

class MouseFarCondition : public Behaviour
{
public:
	MouseFarCondition(float distance) : m_MouseDistance(distance) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts);
private:
	float m_MouseDistance;
};

#pragma endregion

#pragma region Movement

class WanderAction : public Behaviour
{
public:
	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts);
};

#pragma endregion

#pragma region Misc

class SetSpriteColorAction : public Behaviour
{
public:
	SetSpriteColorAction(glm::vec4 color) : m_Color(color) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts);
private:
	glm::vec4 m_Color;
};

#pragma endregion