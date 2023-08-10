#include "NodeMap.h"

#include <Pinecone.h>

using namespace Pinecone;

void NodeMap::Node::ConnectTo(Node* node, float cost)
{
	m_Connections.push_back(Connection(node, cost));
}

NodeMap::NodeMap(Ref<World> world)
{
	// Init nodes
	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			if (world->GetTile(x, y).IsPathable())
				m_Nodes[x][y] = new Node(x, y);
			else
				m_Nodes[x][y] = nullptr;
		}
	}

	// Make connections
	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			Node* node = m_Nodes[x][y];
			if (node)
			{
				// Check if there is a node west of the current node, if so, connect the two
				Node* westNode = x != 0 ? m_Nodes[x - 1][y] : nullptr;
				if (westNode)
				{
					node->ConnectTo(westNode, 1.0f);
					westNode->ConnectTo(node, 1.0f);
				}

				// Check if there is a node south of the current node, if so, connect the two
				Node* southNode = y != 0 ? m_Nodes[x][y - 1] : nullptr;
				if (southNode)
				{
					node->ConnectTo(southNode, 1.0f);
					southNode->ConnectTo(node, 1.0f);
				}

				// Check if there is a node south west of the current, if so, connect the two
				Node* southWestNode = (y != 0 && x != 0) ? m_Nodes[x - 1][y - 1] : nullptr;
				if (southWestNode)
				{
					if (world->GetTile(x - 1, y).IsPathable() && world->GetTile(x, y - 1).IsPathable())
					{
						// The cost is heigher due to being on a diagonal
						node->ConnectTo(southWestNode, 1.4f);
						southWestNode->ConnectTo(node, 1.4f);
					}
				}

				// Check if there is a node south east of the current, if so, connect the two
				Node* southEastNode = (y != 0 && x != WORLD_WIDTH - 1) ? m_Nodes[x + 1][y - 1] : nullptr;
				if (southEastNode)
				{
					if (world->GetTile(x + 1, y).IsPathable() && world->GetTile(x, y - 1).IsPathable())
					{
						// The cost is heigher due to being on a diagonal
						node->ConnectTo(southEastNode, 1.4f);
						southEastNode->ConnectTo(node, 1.4f);
					}
				}
			}
		}
	}
}

NodeMap::~NodeMap()
{
	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			if (m_Nodes[x][y])
				delete m_Nodes[x][y];
		}
	}
}

void NodeMap::OnRender()
{
	float tileSize = 1.0f;
	glm::vec2 offset = { WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f };

	glm::vec4 nodeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.25f);
	glm::vec4 lineColor = glm::vec4(0.75f, 0.2f, 0.2f, 1.25f);

	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			Node* node = m_Nodes[x][y];

			if (node)
			{
				glm::vec2 tilePos = glm::vec2{ x * tileSize, y * tileSize } - offset;
				Renderer2D::DrawQuad(glm::vec3(tilePos, -0.5f), glm::vec2(tileSize, tileSize) * 0.25f, nodeColor);

				for (int i = 0; i < node->GetConnections().size(); i++)
				{
					Node* other = node->GetConnections()[i].GetNode();
					Renderer2D::DrawLine(glm::vec3(node->GetPosition() - offset, -0.75f), glm::vec3(other->GetPosition() - offset, -0.75f), lineColor);
				}
			}
		}
	}
}

NodeMap::Node* NodeMap::GetNode(float x, float y) const
{
	if (x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT)
		return nullptr;
	return m_Nodes[(int)glm::floor(x)][(int)glm::floor(y)];
}
