#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../../World/World.h"

class NodeMap
{
public:
	struct Node
	{
	public:
		struct Connection
		{
		public:
			Connection()
			{
				m_Node = nullptr;
				m_Cost = 0;
			}

			Connection(Node* node, float cost)
				: m_Node(node), m_Cost(cost)
			{}

			Node* GetNode() const { return m_Node; }
			float GetCost() const { return m_Cost; }
		private:
			Node* m_Node;
			float m_Cost;
		};
	public:
		Node(float x, float y)
			: m_Position(x, y)
		{}
		~Node()
		{
			m_Connections.clear();
		}

		void ConnectTo(Node* node, float cost);

		void SetGScore(float score) { m_GScore = score; }
		float GetGScore() const { return m_GScore; }

		void SetHScore(float score) { m_HScore = score; }
		float GetHScore() const { return m_HScore; }

		void SetFScore(float score) { m_FScore = score; }
		float GetFScore() const { return m_FScore; }

		void SetPrevious(Node* previous) { m_Previous = previous; }
		Node* GetPrevious() const { return m_Previous; }

		std::vector<Connection> GetConnections() const { return m_Connections; }
		glm::vec2 GetPosition() const { return m_Position; }
	private:
		std::vector<Connection> m_Connections;
		glm::vec2 m_Position;

		float m_GScore = 0.0f;
		float m_HScore = 0.0f;
		float m_FScore = 0.0f;
		Node* m_Previous = nullptr;
	};
public:
	NodeMap() = default;
	NodeMap(Ref<World> world);
	~NodeMap();

	void OnRender();

	Node* GetNode(int x, int y) const { return m_Nodes[x][y]; };
	Node* GetNode(float x, float y) const;
private:
	Node* m_Nodes[WORLD_WIDTH][WORLD_HEIGHT];
};