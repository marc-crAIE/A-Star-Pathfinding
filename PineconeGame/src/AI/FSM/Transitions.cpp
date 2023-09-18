#include "Transitions.h"

#include "../../Game.h"

TargetFarTransition::TargetFarTransition(Ref<State> targetState, GameObject target, float distance)
	: Transition(targetState), m_GOTarget(target), m_Distance(distance)
{
	m_ConditionFn = [&](GameObject gameObject, Timestep ts) -> bool {
		auto& transform = gameObject.GetComponent<TransformComponent>();
		
		// Check if the target exists, if not it is definitely far away
		if (!Game::GetGOmanager()->Exists(m_GOTarget))
			return true;

		auto& targetTransform = m_GOTarget.GetComponent<TransformComponent>();

		float distance = glm::distance(transform.Translation, targetTransform.Translation);
		if (distance > m_Distance)
			return true;

		return false;
	};
}