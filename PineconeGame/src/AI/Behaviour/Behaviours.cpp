#include "Behaviours.h"

#include "../AIAgent.h"
#include "../../Utils/Utils.h"

BehaviourStatus MouseNearCondition::OnUpdate(GameObject gameObject, Timestep ts)
{
	glm::vec2 mousePos = Utils::MouseToOpenGLCoords();

	auto& transform = gameObject.GetComponent<TransformComponent>();

	float distance = glm::distance(transform.Translation, glm::vec3{ mousePos.x, mousePos.y, 0.0f });

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

	float distance = glm::distance(transform.Translation, glm::vec3{ mousePos.x, mousePos.y, 0.0f });
	if (distance > m_MouseDistance)
	{
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
		float angle = (rand() % 360) * (glm::pi<float>() / 180.0f);
		glm::vec2 dir = { glm::cos(angle), glm::sin(angle) };
		glm::normalize(dir);

		glm::vec2& currDir = agent->GetDirection();
		currDir = dir;

		transform.Translation += glm::vec3{ currDir.x, currDir.y, 0.0f } * agent->GetSpeed() * (float)ts;
		
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

BehaviourStatus FuncAction::OnUpdate(GameObject gameObject, Timestep ts)
{
	return m_Func();
}