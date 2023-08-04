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

class WaitAction : public Behaviour
{
public:
	WaitAction(float time) : m_WaitTime(time) {}

	void OnInit() override;
	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	float m_WaitTime;
	float m_ElapsedTime = 0.0f;
};

class UntilAction : public Behaviour
{
public:
	UntilAction(float time) : m_UntilTime(time) {}

	void OnInit() override;
	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	float m_UntilTime;
	float m_ElapsedTime = 0.0f;
};

class FuncAction : public Behaviour
{
public:
	using ActionFn = std::function<BehaviourStatus(GameObject, Timestep)>;
public:
	FuncAction(ActionFn func) : m_Func(std::move(func)) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	ActionFn m_Func;
};

#pragma endregion

#pragma region Mouse

class MouseNearCondition : public Behaviour
{
public:
	MouseNearCondition(float distance) : m_MouseDistance(distance) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	float m_MouseDistance;
};

class MouseFarCondition : public Behaviour
{
public:
	MouseFarCondition(float distance) : m_MouseDistance(distance) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	float m_MouseDistance;
};

class MouseButtonPressCondition : public Behaviour
{
public:
	MouseButtonPressCondition(MouseCode button) : m_Button(button) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	MouseCode m_Button;
};

class MouseButtonReleaseCondition : public Behaviour
{
public:
	MouseButtonReleaseCondition(MouseCode button) : m_Button(button) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	MouseCode m_Button;
	bool m_ButtonDown = false;
};

#pragma endregion

#pragma region Movement

class WanderAction : public Behaviour
{
public:
	WanderAction(float speed = 1.0f) : m_Speed(speed) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	float m_Speed;
};

#pragma endregion

#pragma region Misc

class SetSpriteColorAction : public Behaviour
{
public:
	SetSpriteColorAction(glm::vec4 color) : m_Color(color) {}

	BehaviourStatus OnUpdate(GameObject gameObject, Timestep ts) override;
private:
	glm::vec4 m_Color;
};

#pragma endregion