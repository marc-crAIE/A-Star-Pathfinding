#pragma once

#include <Pinecone.h>

using namespace Pinecone;

class ResourceManager
{
public:
	static bool LoadTexture(const std::string& path);
	static Ref<Texture2D> GetTexture(const std::string& path);
private:
	static std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
};