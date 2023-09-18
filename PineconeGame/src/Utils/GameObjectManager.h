#pragma once

#include <Pinecone.h>

using namespace Pinecone;

class GameObjectManager
{
public:
	GameObjectManager(Ref<Scene> scene)
		: m_Scene(scene) {}

	void OnUpdate(Timestep ts);

	bool Exists(GameObject go);

	GameObject GetGameObjectFromTag(const std::string& tag);
	std::vector<GameObject> GetGameObjectsFromTag(const std::string& tag);

	GameObject Create(const std::string& tag = "GameObject");
	void Destroy(GameObject go, float time = 0.0f);
private:
	Ref<Scene> m_Scene;

	std::unordered_map<std::string, std::unordered_map<UUID, GameObject>> m_GameObjectsByTag;
	std::vector<std::pair<GameObject, float>> m_DestroyList;
};