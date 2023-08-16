#pragma once

#include <Pinecone.h>

#include "Behaviour/Behaviours.h"
#include "Pathfinding/PathAlgorithm.h"

using namespace Pinecone;

class AIAgent : public ScriptableGameObject
{
public:
	~AIAgent();

	void SetDirection(glm::vec2& dir) { m_Direction = dir; }
	glm::vec2& GetDirection() { return m_Direction; }

	void SetSpeed(float speed) { m_Speed = speed; }
	float& GetSpeed() { return m_Speed; }

	void SetDestination(NodeMap::Node* node);
	void SetDestination(NodeMap::Node* start, NodeMap::Node* end);
	bool FollowPath(Timestep ts);

	Ref<Pathfinding::PathAlgorithm> GetPathAlgo() const { return m_PathAlgo; }
protected:
	glm::vec2 m_Direction = glm::vec2(0.0f);
	float m_Speed = 1.0f;

	Behaviour* m_BehaviourTree;
	Ref<Pathfinding::PathAlgorithm> m_PathAlgo;
};