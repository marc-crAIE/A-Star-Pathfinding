#include "AIAgent.h"

#include "../Game.h"

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
		m_PathAlgo->Search(Game::GetNodeMap(), Game::GetNodeMap()->GetNode(glm::floor(position.x), glm::floor(position.y)), node);
		return;
	}
	
	m_PathAlgo->ResetPath();
}
