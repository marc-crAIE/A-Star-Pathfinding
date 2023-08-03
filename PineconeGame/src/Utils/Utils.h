#pragma once

#include <Pinecone.h>

using namespace Pinecone;

#include "../Game.h"

namespace Utils
{

	static glm::vec2 MouseToOpenGLCoords()
	{
		glm::vec2 mousePos = Input::GetMousePosition();
		auto& camera = Game::GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		uint32_t windowWidth = Application::Get().GetWindow().GetWidth();
		uint32_t windowHeight = Application::Get().GetWindow().GetHeight();

		float orthoSize = camera.GetOrthographicSize();
		float aspectRatio = camera.GetAspectRatio();

		float orthoWidth = orthoSize * aspectRatio;
		float orthoHeight = orthoSize;

		float xPos = (mousePos.x / windowWidth) * orthoWidth;
		float yPos = (mousePos.y / windowHeight) * orthoHeight;

		xPos -= orthoWidth / 2.0f;
		yPos -= orthoHeight / 2.0f;

		return { xPos, yPos };
	}

}