#pragma once

#include <Pinecone.h>

#include "World/World.h"
#include "AI/NodeMap/NodeMap.h"

#include "AI/Pathfinding/Dijkstras.h"
#include "AI/Pathfinding/AStar.h"

using namespace Pinecone;

class Game : public Layer
{
public:
	Game();
	~Game() = default;

	void OnAttach() override;
	void OnEvent(Event& e) override;

	void OnUpdate(Timestep ts) override;

	void SpawnKnight(float x, float y);

	static Game& Get() { return *s_Instance; }
	static Ref<Scene> GetScene() { return s_Instance->m_ActiveScene; }
	static Ref<NodeMap> GetNodeMap() { return s_Instance->m_NodeMap; }
private:
	bool OnWindowResized(WindowResizeEvent& e);
private:
	Ref<Scene> m_ActiveScene;

	Ref<World> m_World;
	Ref<NodeMap> m_NodeMap;

	GameObject m_Camera;

	bool m_MousePressed = false;

	// TEMPORARY
	GameObject m_Knight;
	Pathfinding::AStar m_PathAlgo;

	NodeMap::Node* m_StartNode = nullptr;
	NodeMap::Node* m_EndNode = nullptr;
private:
	static Game* s_Instance;
};