#include "EnemyKnight.h"

#include "../../Game.h"
#include "../../AI/Pathfinding/AStar.h"
#include "../../Utils/Utils.h"

EnemyKnight::EnemyKnight(bool captain)
{
	m_Type == captain ? CAPTAIN : KNIGHT;
}

void EnemyKnight::OnCreate()
{
	m_Speed = 3.0f;
	m_Castle = Game::GetScene()->GetGameObjectByTag("Castle");
	m_PathAlgo = CreateRef<Pathfinding::AStar>();

	// Find the first land node
	auto& transform = GetComponent<TransformComponent>();
	glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0) + glm::vec2(0.5f, 0.5f);
	glm::vec2 nodePos = glm::vec2(transform.Translation) + offset;
	glm::vec2 dirToCenter = glm::normalize(glm::vec2(-transform.Translation));

	NodeMap::Node* node = nullptr;
	do
	{
		node = Game::GetNodeMap()->GetNode(nodePos.x, nodePos.y);
		nodePos += dirToCenter;
	} while (!node);

	int x = Utils::RandomInt((WORLD_WIDTH / 2) - 3, (WORLD_WIDTH / 2) + 3);
	int y = Utils::RandomInt((WORLD_HEIGHT / 2) - 3, (WORLD_HEIGHT / 2) + 3);
	SetDestination(node, Game::GetNodeMap()->GetNode(x, y));
}

void EnemyKnight::OnUpdate(Timestep ts)
{
	if (m_PathAlgo->HasPath())
		FollowPath(ts);
}
