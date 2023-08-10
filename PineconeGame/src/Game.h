#pragma once

#include <Pinecone.h>
#include <Pinecone/Events/KeyEvent.h>

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
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	Ref<Scene> m_ActiveScene;

	Ref<World> m_World;
	Ref<NodeMap> m_NodeMap;

	GameObject m_Camera;

	bool m_ShowNodeMap = false;
private:
	static Game* s_Instance;
};