#pragma once

#include <vector>
#include <Pinecone.h>

#include "../NodeMap/NodeMap.h"

using namespace Pinecone;

namespace Pathfinding
{
	class PathAlgorithm
	{
	public:
		virtual void Search(Ref<NodeMap> nodeMap, NodeMap::Node* startNode, NodeMap::Node* endNode);

		void ResetPath();

		std::vector<NodeMap::Node*> GetPath() const { return m_Path; }
		NodeMap::Node* GetCurrentPathNode() const;
		NodeMap::Node* GetNextPathNode();

		bool HasPath() const { return m_Path.size() > 1; }
	protected:
		std::vector<NodeMap::Node*> m_Path;
		int m_CurrentPathIndex = 0;
	};
}