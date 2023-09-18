#pragma once

#include <Pinecone.h>
#include <Pinecone/Events/KeyEvent.h>

#include "World/World.h"
#include "AI/NodeMap/NodeMap.h"

#include "AI/Pathfinding/Dijkstras.h"
#include "AI/Pathfinding/AStar.h"

#include "Utils/GameObjectManager.h"

using namespace Pinecone;

class Game : public Layer
{
public:
	Game();
	~Game() = default;

	void OnAttach() override;
	void OnEvent(Event& e) override;

	void OnUpdate(Timestep ts) override;
	void OnImGuiRender() override;

	void DestroyGameObject(GameObject gameObject);

	void SpawnKnight(float x, float y);
	void SpawnEnemies(float x, float y);
	void SpawnShip();

	static Game& Get() { return *s_Instance; }
	static Ref<Scene> GetScene() { return s_Instance->m_ActiveScene; }
	static Ref<GameObjectManager> GetGOmanager() { return s_Instance->m_GameObjectManager; }
	static Ref<World> GetWorld() { return s_Instance->m_World; }
	static Ref<NodeMap> GetNodeMap() { return s_Instance->m_NodeMap; }
private:
	bool OnWindowResized(WindowResizeEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	Ref<Scene> m_ActiveScene;

	Ref<World> m_World;
	Ref<NodeMap> m_NodeMap;

	float m_SpawnNextShip = 20.0f;
	float m_SpawnShipElapsed = 0.0f;

	float m_SpawnNextKnight = 30.0f;
	float m_SpawnKnightElapsed = 0.0f;

	GameObject m_Camera;
	Ref<GameObjectManager> m_GameObjectManager;

	bool m_ShowNodeMap = false;
private:
	static Game* s_Instance;
};