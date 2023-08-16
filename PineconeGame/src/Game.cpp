#include "Game.h"

#include "Entity/Knight.h"
#include "Entity/Enemies/EnemyKnight.h"
#include "Entity/Ship.h"
#include "Utils/Utils.h"
#include "Utils/ResourceManager.h"

Game* Game::s_Instance = nullptr;

Game::Game() 
	: Layer("GameLayer")
{
	PC_ASSERT(!s_Instance, "There is already an instance of the game!");
	s_Instance = this;

	srand(time(NULL));
}

void Game::OnAttach()
{
	RenderCommand::SetClearColor({ 0.165f, 0.49f, 0.459f, 1.0f });

	m_ActiveScene = CreateRef<Scene>();

	m_World = CreateRef<World>();
	m_NodeMap = CreateRef<NodeMap>(m_World);

	m_Camera = m_ActiveScene->CreateGameObject("Camera");
	auto& cc = m_Camera.AddComponent<CameraComponent>();

	cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	cc.Camera.SetOrthographic(40.0f, -1.0f, 1.0f);

	for (int i = 0; i < 10; i++)
		SpawnKnight(3, 3);

	SpawnShip();
}

void Game::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(Game::OnWindowResized));
	dispatcher.Dispatch<KeyReleasedEvent>(PC_BIND_EVENT_FN(Game::OnKeyReleased));
}

void Game::OnUpdate(Timestep ts)
{
	Renderer2D::ResetStats();
	RenderCommand::Clear();

	//PC_INFO("FPS: {0}", 1 / ts);

	m_ActiveScene->OnUpdate(ts);

	// If there are any Game Objects to be destroyed after updating our scripts. Destroy all of them
	for (auto gameObject : m_GameObjectsToDestroy)
	{
		// Destroy the Game Object in the active scene
		m_ActiveScene->DestroyGameObject(gameObject);
	}
	// Clear the vector as we cannot destroy Game Objects that no longer exist in the scene
	m_GameObjectsToDestroy.clear();

	Renderer2D::BeginScene(m_Camera.GetComponent<CameraComponent>().Camera);
	m_World->OnRender();
	if (m_ShowNodeMap)
		m_NodeMap->OnRender();

	Renderer2D::EndScene();
}

void Game::DestroyGameObject(GameObject gameObject)
{
	m_GameObjectsToDestroy.push_back(gameObject);
}

void Game::SpawnKnight(float x, float y)
{
	auto knight = m_ActiveScene->CreateGameObject("Knight");
	knight.AddComponent<NativeScriptComponent>().Bind<Knight>();

	auto& transform = knight.GetComponent<TransformComponent>();
	transform.Translation = glm::vec3(x, y, 0.0f);

	auto& sprite = knight.AddComponent<SpriteComponent>();
	sprite.Texture = ResourceManager::GetTexture("assets/textures/entities/knight/Knight_Idle.png");
}

void Game::SpawnEnemies(float x, float y)
{
	auto captain = m_ActiveScene->CreateGameObject("Enemy Captain");
	captain.AddComponent<NativeScriptComponent>().Bind<EnemyKnight>();

	auto& captainTransform = captain.GetComponent<TransformComponent>();
	captainTransform.Translation = glm::vec3(x, y, 0.0f);

	auto& captainSprite = captain.AddComponent<SpriteComponent>();
	captainSprite.Texture = ResourceManager::GetTexture("assets/textures/entities/enemy_captain/Enemy_Captain_Idle.png");

	int enemyCount = rand() % 4 + 1;

	for (int i = 0; i < enemyCount; i++)
	{
		auto knight = m_ActiveScene->CreateGameObject("Enemy Knight");
		knight.AddComponent<NativeScriptComponent>().Bind<EnemyKnight>();

		auto& transform = knight.GetComponent<TransformComponent>();
		transform.Translation = glm::vec3(x, y, 0.0f);

		auto& sprite = knight.AddComponent<SpriteComponent>();
		sprite.Texture = ResourceManager::GetTexture("assets/textures/entities/enemy_knight/Enemy_Knight_Idle.png");
	}
}

void Game::SpawnShip()
{
	auto ship = m_ActiveScene->CreateGameObject("Enemy Ship");
	ship.AddComponent<NativeScriptComponent>().Bind<Ship>();

	glm::vec4 screenBounds = Utils::GetScreenBounds();
	int x = 0;
	int y = 0;

	int edge = rand() % 4;
	switch (edge)
	{
	case 0:
		x = screenBounds.x;
		y = Utils::RandomFloat(screenBounds.z, screenBounds.w);
		break;
	case 1:
		x = screenBounds.y;
		y = Utils::RandomFloat(screenBounds.z, screenBounds.w);
		break;
	case 2:
		x = Utils::RandomFloat(screenBounds.x, screenBounds.y);
		y = screenBounds.z;
		break;
	case 3:
		x = Utils::RandomFloat(screenBounds.x, screenBounds.y);
		y = screenBounds.w;
		break;
	}

	auto& transform = ship.GetComponent<TransformComponent>();
	transform.Translation = glm::vec3(x, y, 0.0f);
	transform.Scale = glm::vec3(2.375f, 2.875f, 1.0f);

	auto& sprite = ship.AddComponent<SpriteComponent>();
	sprite.Texture = ResourceManager::GetTexture("assets/textures/entities/ship/ship.png");
}

bool Game::OnWindowResized(WindowResizeEvent& e)
{
	m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
	return false;
}

bool Game::OnKeyReleased(KeyReleasedEvent& e)
{
	if (e.GetKeyCode() == Key::F1)
	{
		m_ShowNodeMap = !m_ShowNodeMap;
		return true;
	}
	return false;
}
