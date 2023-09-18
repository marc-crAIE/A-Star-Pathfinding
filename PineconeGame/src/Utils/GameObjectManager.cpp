#include "GameObjectManager.h"

void GameObjectManager::OnUpdate(Timestep ts)
{
    PC_PROFILE_FUNCTION();

    for (int i = 0; i < m_DestroyList.size(); i++)
    {
        if (m_DestroyList[i].second <= 0.0f)
        {
            GameObject go = m_DestroyList[i].first;
            m_DestroyList.erase(m_DestroyList.begin() + i);

            if (Exists(go))
            {
                std::string tag = go.GetComponent<TagComponent>();
                if (m_GameObjectsByTag.find(tag) != m_GameObjectsByTag.end())
                    m_GameObjectsByTag[tag].erase(go.GetUUID());

                m_Scene->DestroyGameObject(go);
            }
        }
        else
        {
            m_DestroyList[i].second -= ts;
        }
    }
}

bool GameObjectManager::Exists(GameObject go)
{
    return m_Scene->GameObjectExists(go);
}

GameObject GameObjectManager::GetGameObjectFromTag(const std::string& tag)
{
    if (m_GameObjectsByTag.find(tag) != m_GameObjectsByTag.end())
        return m_GameObjectsByTag[tag].begin()->second;
    return {};
}

std::vector<GameObject> GameObjectManager::GetGameObjectsFromTag(const std::string& tag)
{
    if (m_GameObjectsByTag.find(tag) != m_GameObjectsByTag.end())
    {
        std::vector<GameObject> result;
        for (auto& [UUID, go] : m_GameObjectsByTag[tag])
            result.push_back(go);

        return result;
    }

    return std::vector<GameObject>();
}

GameObject GameObjectManager::Create(const std::string& tag)
{
    GameObject go = m_Scene->CreateGameObject(tag);
    m_GameObjectsByTag[tag][go.GetUUID()] = go;
    return go;
}

void GameObjectManager::Destroy(GameObject go, float time)
{
    m_DestroyList.push_back(std::make_pair(go, time));
}
