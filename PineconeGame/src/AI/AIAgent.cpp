#include "AIAgent.h"

#include <Pinecone.h>

#include "../Game.h"
#include "../Utils/Utils.h"

using namespace Pinecone;

AIAgent::~AIAgent()
{
	if (m_BehaviourTree)
		delete m_BehaviourTree;
}

void AIAgent::SetDestination(NodeMap::Node* node)
{
	if (node)
	{

		glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0) + glm::vec2(0.5f, 0.5f);
		auto& position = glm::vec2(GetComponent<TransformComponent>().Translation) + offset;
		SetDestination(Game::GetNodeMap()->GetNode(glm::floor(position.x), glm::floor(position.y)), node);
		return;
	}
	
	m_PathAlgo->ResetPath();
}

void AIAgent::SetDestination(NodeMap::Node* start, NodeMap::Node* end)
{
	if (start && end)
	{
		glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0) + glm::vec2(0.5f, 0.5f);
		auto& position = glm::vec2(GetComponent<TransformComponent>().Translation) + offset;
		m_PathAlgo->Search(Game::GetNodeMap(), start, end);
		return;
	}

	m_PathAlgo->ResetPath();
}

bool AIAgent::FollowPath(Timestep ts)
{
	if (GetPathAlgo()->HasPath())
	{
		glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0);

		auto& transform = GetComponent<TransformComponent>();
		auto knightPos = glm::vec2(GetComponent<TransformComponent>().Translation) + offset;
		auto path = GetPathAlgo();

		NodeMap::Node* currNode = path->GetCurrentPathNode();

		if (currNode == path->GetPath().front())
			currNode = path->GetNextPathNode();

		float dist = glm::distance(knightPos, currNode->GetPosition());
		glm::vec2 dir = glm::normalize(currNode->GetPosition() - knightPos);

		if ((dist - (m_Speed * ts)) > 0)
		{
			transform.Translation += glm::vec3(m_Speed * dir * (float)ts, 0.0f);
			//transform.Translation.z = Utils::GetWorldZ(transform.Translation.y);
		}
		else
		{
			if (currNode == path->GetPath()[path->GetPath().size() - 1])
			{
				path->ResetPath();
				return true;
			}

			currNode = path->GetNextPathNode();
		}

		return true;
	}
	return false;
}
