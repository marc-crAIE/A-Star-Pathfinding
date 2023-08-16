#pragma once

#include <Pinecone.h>

using namespace Pinecone;

class Ship : public ScriptableGameObject
{
public:
	Ship() = default;
	~Ship() = default;

	void OnCreate() override;
	void OnUpdate(Timestep ts) override;
private:
	float m_Speed;
	glm::vec3 m_Direction;
};