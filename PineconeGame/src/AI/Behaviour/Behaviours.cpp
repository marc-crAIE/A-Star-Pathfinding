#include "Behaviours.h"

#include "../AIAgent.h"
#include "../../Utils/Utils.h"

void WaitAction::OnInit()
{
	m_ElapsedTime = 0.0f;
}

BehaviourStatus WaitAction::OnUpdate(GameObject gameObject, Timestep ts)
{
	if (m_ElapsedTime >= m_WaitTime)
		return BH_SUCCESS;

	m_ElapsedTime += ts;
	return BH_RUNNING;
}

void UntilAction::OnInit()
{
	m_ElapsedTime = 0.0f;
}

BehaviourStatus UntilAction::OnUpdate(GameObject gameObject, Timestep ts)
{
	if (m_ElapsedTime >= m_UntilTime)
		return BH_SUCCESS;

	m_ElapsedTime += ts;
	return BH_PENDING;
}

BehaviourStatus FuncAction::OnUpdate(GameObject gameObject, Timestep ts)
{
	return m_Func(gameObject, ts);
}

BehaviourStatus MouseNearCondition::OnUpdate(GameObject gameObject, Timestep ts)
{
	glm::vec2 mousePos = Utils::MouseToOpenGLCoords();

	auto& transform = gameObject.GetComponent<TransformComponent>();
	float distance = glm::distance(transform.Translation, glm::vec3{ mousePos.x, -mousePos.y, 0.0f });

	if (distance <= m_MouseDistance)
	{
		return BH_SUCCESS;
	}
	return BH_FAILURE;
}

BehaviourStatus MouseFarCondition::OnUpdate(GameObject gameObject, Timestep ts)
{
	glm::vec2 mousePos = Utils::MouseToOpenGLCoords();

	auto& transform = gameObject.GetComponent<TransformComponent>();

	float distance = glm::distance(transform.Translation, glm::vec3{ mousePos.x, -mousePos.y, 0.0f });
	if (distance > m_MouseDistance)
	{
		return BH_SUCCESS;
	}
	return BH_FAILURE;
}

BehaviourStatus MouseButtonPressCondition::OnUpdate(GameObject gameObject, Timestep ts)
{
	return Input::IsMouseButtonPressed(m_Button) ? BH_SUCCESS : BH_FAILURE;
}

BehaviourStatus MouseButtonReleaseCondition::OnUpdate(GameObject gameObject, Timestep ts)
{
	if (Input::IsMouseButtonPressed(m_Button))
		m_ButtonDown = true;
	else if (Input::IsMouseButtonReleased(m_Button) && m_ButtonDown)
	{
		m_ButtonDown = false;
		return BH_SUCCESS;
	}
	return BH_FAILURE;
}

BehaviourStatus WanderAction::OnUpdate(GameObject gameObject, Timestep ts)
{
	auto& transform = gameObject.GetComponent<TransformComponent>();
	AIAgent* agent = dynamic_cast<AIAgent*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (agent)
	{
		// There is no current path, find a new path to wander to
		if (!agent->GetPathAlgo()->HasPath())
		{
			int posX = rand() % WORLD_WIDTH;
			int posY = rand() % WORLD_HEIGHT;

			NodeMap::Node* node = Game::GetNodeMap()->GetNode(posX, posY);
			if (!node)
				return BH_FAILURE;

			agent->SetDestination(node);
			if (!agent->GetPathAlgo()->HasPath())
				return BH_FAILURE;
		}

		agent->FollowPath(ts);
		return BH_SUCCESS;
	}
	return BH_FAILURE;
}

BehaviourStatus WanderAroundObjectAction::OnUpdate(GameObject gameObject, Timestep ts)
{
	auto& transform = gameObject.GetComponent<TransformComponent>();
	AIAgent* agent = dynamic_cast<AIAgent*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (agent)
	{
		// There is no current path, find a new path to wander to
		if (!agent->GetPathAlgo()->HasPath())
		{
			glm::vec3 offset = glm::vec3(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0, 0.0f) + glm::vec3(0.5f, 0.5f, 0.0f);

			float angle = (rand() % 360) * glm::pi<float>() / 180.0f;
			glm::vec2 dir = glm::vec2(glm::cos(angle), glm::sin(angle));
			glm::normalize(dir);

			auto& targetTransform = m_Target.GetComponent<TransformComponent>();
			glm::vec2 targetPos = targetTransform.Translation + offset;
			targetPos += dir * glm::vec2(targetTransform.Scale);

			NodeMap::Node* node = Game::GetNodeMap()->GetNode(targetPos.x, targetPos.y);
			if (!node)
				return BH_FAILURE;

			agent->SetDestination(node);
			if (!agent->GetPathAlgo()->HasPath())
				return BH_FAILURE;
		}

		agent->FollowPath(ts);
		return BH_SUCCESS;
	}

	return BH_FAILURE;
}

BehaviourStatus SetSpriteColorAction::OnUpdate(GameObject gameObject, Timestep ts)
{
	if (gameObject.HasComponent<SpriteComponent>())
	{
		gameObject.GetComponent<SpriteComponent>().Color = m_Color;
		return BH_SUCCESS;
	}
	return BH_FAILURE;
}