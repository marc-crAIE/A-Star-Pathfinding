#include "Pathfinding.h"

void Pathfinding::PathAlgorithm::Search(Ref<NodeMap> nodeMap, NodeMap::Node* startNode, NodeMap::Node* endNode)
{
	ResetPath();
	m_Path.push_back(startNode);
}

NodeMap::Node* Pathfinding::PathAlgorithm::GetNextPathNode(float x, float y) const
{
	return m_Path[m_CurrentPathIndex];
}

void Pathfinding::PathAlgorithm::ResetPath()
{
	m_Path.clear();
	m_CurrentPathIndex = 0;
}
