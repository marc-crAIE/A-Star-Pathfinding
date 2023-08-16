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

	void DestroyGameObject(GameObject gameObject);

	void SpawnKnight(float x, float y);
	void SpawnEnemies(float x, float y);
	void SpawnShip();

	static Game& Get() { return *s_Instance; }
	static Ref<Scene> GetScene() { return s_Instance->m_ActiveScene; }
	static Ref<World> GetWorld() { return s_Instance->m_World; }
	static Ref<NodeMap> GetNodeMap() { return s_Instance->m_NodeMap; }
private:
	bool OnWindowResized(WindowResizeEvent& e);
	bool OnKeyReleased(KeyReleasedEvent& e);
private:
	Ref<Scene> m_ActiveScene;

	Ref<World> m_World;
	Ref<NodeMap> m_NodeMap;

	GameObject m_Camera;

	std::vector<GameObject> m_GameObjectsToDestroy;

	bool m_ShowNodeMap = false;
private:
	static Game* s_Instance;
};