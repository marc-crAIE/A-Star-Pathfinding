#pragma once

#include "PathAlgorithm.h"

namespace Pathfinding
{
	class AStar : public PathAlgorithm
	{
	public:
		void Search(Ref<NodeMap> nodeMap, NodeMap::Node* startNode, NodeMap::Node* endNode) override;
	};
}