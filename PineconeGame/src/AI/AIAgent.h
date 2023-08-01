#pragma once

#include <Pinecone.h>

#include "Behaviour/Behaviours.h"
#include "Pathfinding/Pathfinding.h"

using namespace Pinecone;

class AIAgent : public ScriptableGameObject
{
public:
	~AIAgent();

	void SetDirection(glm::vec2& dir) { m_Direction = dir; }
	glm::vec2& GetDirection() { return m_Direction; }

	void SetSpeed(float speed) { m_Speed = speed; }
	float& GetSpeed() { return m_Speed; }
protected:
	glm::vec2 m_Direction = glm::vec2(0.0f);
	float m_Speed = 1.0f;

	Behaviour* m_BehaviourTree;
	Pathfinding::PathAlgorithm m_PathAlgo;
};