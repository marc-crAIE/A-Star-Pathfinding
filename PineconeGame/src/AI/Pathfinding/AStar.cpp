#include "AStar.h"

#include <algorithm>
#include <glm/glm.hpp>

static bool SortNodes(NodeMap::Node* a, NodeMap::Node* b)
{
	return a->GetFScore() < b->GetFScore();
}

static float Heuristic(NodeMap::Node* node, NodeMap::Node* endNode)
{
	return glm::distance(node->GetPosition(), endNode->GetPosition());
}

void Pathfinding::AStar::Search(Ref<NodeMap> nodeMap, NodeMap::Node* startNode, NodeMap::Node* endNode)
{
	ResetPath();

	if (!startNode || !endNode)
	{
		PC_ERROR("Start node or end node is not set!");
		m_Path.push_back(startNode);
		return;
	}

	if (startNode == endNode)
	{
		m_Path.push_back(startNode);
		return;
	}

	// Init the start node
	startNode->SetGScore(0);
	startNode->SetPrevious(nullptr);

	// Lists for storing nodes
	std::vector<NodeMap::Node*> openList;
	std::vector<NodeMap::Node*> closedList;

	// If a path was successfully found
	bool foundPath = false;

	// Add the starting node to the open list
	openList.push_back(startNode);

	while (!openList.empty())
	{
		// Sort the open list based on the SortNodes function
		std::sort(openList.begin(), openList.end(), SortNodes);

		// Set the current node to the first node in the open list
		NodeMap::Node* currentNode = openList.front();
		// Remove the current node from the open list
		openList.erase(openList.begin());
		// Then add the current node to the closed list
		closedList.push_back(currentNode);

		// If the end node was added to the closed list, the path has been found!
		if (currentNode == endNode)
		{
			foundPath = true;
			break;
		}

		for (NodeMap::Node::Connection c : currentNode->GetConnections())
		{
			// If the target node is in the closed list, ignore it
			if (std::find(closedList.begin(), closedList.end(), c.GetNode()) != closedList.end())
				continue;

			float gScore = currentNode->GetGScore() + c.GetCost();
			float hScore = Heuristic(c.GetNode(), endNode);
			float fScore = gScore + hScore;

			// If the target node is not in the open list, add it
			if (std::find(openList.begin(), openList.end(), c.GetNode()) == openList.end())
			{
				c.GetNode()->SetGScore(gScore);
				c.GetNode()->SetHScore(hScore);
				c.GetNode()->SetFScore(fScore);
				// Set the targets node's previous to the current node
				c.GetNode()->SetPrevious(currentNode);

				// Insert the node to the list and keep it sorted
				auto insertPos = openList.end();
				for (auto itr = openList.begin(); itr != openList.end(); itr++)
				{
					if (c.GetNode()->GetFScore() < (*itr)->GetFScore())
					{
						insertPos = itr;
						break;
					}
				}

				// Insert the node
				openList.insert(insertPos, c.GetNode());
			}
			else
			{
				// Compare the new G score to the old one
				if (fScore < c.GetNode()->GetFScore())
				{
					c.GetNode()->SetGScore(gScore);
					c.GetNode()->SetFScore(fScore);
					// Set the targets node's previous to the current node
					c.GetNode()->SetPrevious(currentNode);
				}
			}
		}
	}

	// If a path had been found
	if (foundPath)
	{
		// Create the path in reverse order
		NodeMap::Node* currentNode = endNode;
		while (currentNode)
		{
			// Add the current node to the beginning of the path
			m_Path.insert(m_Path.begin(), currentNode);
			// Set the current node to the previous node
			currentNode = currentNode->GetPrevious();
		}
	}
	else
	{
		m_Path.push_back(startNode);
	}
}
