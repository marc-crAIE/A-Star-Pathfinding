#include "ResourceManager.h"

std::unordered_map<std::string, Ref<Texture2D>> ResourceManager::m_Textures;

bool ResourceManager::LoadTexture(const std::string& path)
{
	if (m_Textures.find(path) != m_Textures.end())
		return true;

	Ref<Texture2D> texture = Texture2D::Create(path);

	if (!texture->IsLoaded())
		return false;

	m_Textures[path] = texture;
	return true;
}

Ref<Texture2D> ResourceManager::GetTexture(const std::string& path)
{
	if (LoadTexture(path))
		return m_Textures[path];

	PC_ERROR("No texture is loaded! \"{0}\"", path);
	return Ref<Texture2D>();
}
