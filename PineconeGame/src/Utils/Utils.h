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

	static glm::vec4 GetScreenBounds()
	{
		// Borrowing the camera game object from the scene. Normally would not do this if the camera moved around
		// the scene as UI elements would not follow unless we factor in the camera's translation.
		// Normally would create a seperate scene camera that just stays at a fixed position.
		auto camera = Game::GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		//TODO: Probably have a function inside of SceneCamera that returns a Bounds object to get these values
		//		as this is used in many places.

		// Get the orthographic screen bounds
		float orthoSize = camera.GetOrthographicSize();
		float aspectRatio = camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		return glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom);
	}

	static glm::vec2 ScreenPosToWorldPos(glm::vec2 pos)
	{
		glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0);
		return glm::vec2(pos.x + offset.x, pos.y + offset.y);
	}

	static float GetWorldZ(float y)
	{
		return ((float)WORLD_HEIGHT / y) / WORLD_HEIGHT;
	}

	static int RandomInt(int min, int max)
	{
		int random = rand() % max + min;
		return random;
	}

	static float RandomFloat(float min, float max)
	{
		float random = ((float)rand()) / (float)RAND_MAX;
		float range = max - min;
		return (random * range) + min;
	}
}