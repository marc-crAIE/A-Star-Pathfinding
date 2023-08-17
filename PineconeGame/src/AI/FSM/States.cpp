#include "States.h"

#include "../AIAgent.h"
#include "../../Game.h"
#include "../../Utils/Utils.h"

void WanderState::OnUpdate(GameObject gameObject, Timestep ts)
{
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
				return;

			agent->SetDestination(node);
			if (!agent->GetPathAlgo()->HasPath())
				return;
		}

		agent->FollowPath(ts);
	}
}

void FollowState::OnUpdate(GameObject gameObject, Timestep ts)
{
	AIAgent* agent = dynamic_cast<AIAgent*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (agent && (entt::entity)m_Target != entt::null)
	{
		if (!agent->GetPathAlgo()->HasPath())
		{
			auto& targetTransform = m_Target.GetComponent<TransformComponent>();
			glm::vec2 targetPos = Utils::ScreenPosToWorldPos(targetTransform.Translation);

			NodeMap::Node* node = Game::GetNodeMap()->GetNode(targetPos.x, targetPos.y);
			if (!node)
				return;

			agent->SetDestination(node);
			if (!agent->GetPathAlgo()->HasPath())
				return;
		}

		agent->FollowPath(ts);
	}
}
