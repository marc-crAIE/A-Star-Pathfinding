#include "Game.h"

#include "Entity/Knight.h"
#include "Entity/Enemies/EnemyKnight.h"
#include "Entity/Ship.h"
#include "Utils/Utils.h"
#include "Utils/ResourceManager.h"

#include <imgui/imgui.h>

Game* Game::s_Instance = nullptr;

Game::Game() 
	: Layer("GameLayer")
{
	PC_PROFILE_FUNCTION();

	PC_ASSERT(!s_Instance, "There is already an instance of the game!");
	s_Instance = this;

	srand(time(NULL));
}

void Game::OnAttach()
{
	PC_PROFILE_FUNCTION();

	RenderCommand::SetClearColor({ 0.165f, 0.49f, 0.459f, 1.0f });

	m_ActiveScene = CreateRef<Scene>();
	m_GameObjectManager = CreateRef<GameObjectManager>(m_ActiveScene);

	m_World = CreateRef<World>();
	m_NodeMap = CreateRef<NodeMap>(m_World);

	m_Camera = m_GameObjectManager->Create("Camera");
	auto& cc = m_Camera.AddComponent<CameraComponent>();

	cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	cc.Camera.SetOrthographic(40.0f, -1.0f, 1.0f);

	for (int i = 0; i < 10; i++)
	{
		SpawnKnight(0, -2);
	}
}

void Game::OnEvent(Event& e)
{
	PC_PROFILE_FUNCTION();

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(Game::OnWindowResized));
	dispatcher.Dispatch<KeyReleasedEvent>(PC_BIND_EVENT_FN(Game::OnKeyReleased));
}

void Game::OnUpdate(Timestep ts)
{
	PC_PROFILE_FUNCTION();

	Renderer2D::ResetStats();
	RenderCommand::Clear();

	PC_INFO("FPS: {0}", 1 / ts);

	if (m_SpawnKnightElapsed >= m_SpawnNextKnight)
	{
		SpawnKnight(0, -2);
		m_SpawnKnightElapsed = 0.0f;
	}
	else
		m_SpawnShipElapsed += ts;

	if (m_SpawnShipElapsed >= m_SpawnNextShip)
	{
		SpawnShip();
		m_SpawnShipElapsed = 0.0f;
	}
	else
		m_SpawnShipElapsed += ts;

	m_ActiveScene->OnUpdate(ts);
	m_GameObjectManager->OnUpdate(ts);

	Renderer2D::BeginScene(m_Camera.GetComponent<CameraComponent>().Camera);
	m_World->OnRender();
	if (m_ShowNodeMap)
		m_NodeMap->OnRender();

	Renderer2D::EndScene();
}

void Game::OnImGuiRender()
{
#ifdef PC_DEBUG

	ImGui::Begin("Info");

	ImGui::Text("Mouse Tile Position: %s", glm::to_string(Utils::MouseToOpenGLCoords()).c_str());

	glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0) + glm::vec2(0.5f, -0.5f);
	glm::vec2 mousePos = Utils::MouseToOpenGLCoords() + offset;
	NodeMap::Node* node = m_NodeMap->GetNode(mousePos.x, WORLD_HEIGHT - mousePos.y);
	std::string nodeInfo = node ? glm::to_string(node->GetPosition()) : "null";
	ImGui::Text("Node: %s", nodeInfo.c_str());

	ImGui::End();

#endif
}

void Game::DestroyGameObject(GameObject gameObject)
{
	PC_PROFILE_FUNCTION();

	m_GameObjectManager->Destroy(gameObject);
}

void Game::SpawnKnight(float x, float y)
{
	PC_PROFILE_FUNCTION();

	auto knight = m_GameObjectManager->Create("Knight");
	knight.AddComponent<NativeScriptComponent>().Bind<Knight>();

	auto& transform = knight.GetComponent<TransformComponent>();
	transform.Translation = glm::vec3(x, y, 0.0f);

	auto& sprite = knight.AddComponent<SpriteComponent>();
	sprite.Texture = ResourceManager::GetTexture("assets/textures/entities/knight/Knight_Idle.png");
}

void Game::SpawnEnemies(float x, float y)
{
	PC_PROFILE_FUNCTION();

	auto captain = m_GameObjectManager->Create("Enemy Captain");
	captain.AddComponent<NativeScriptComponent>().Bind<EnemyKnight>(true);

	auto& captainTransform = captain.GetComponent<TransformComponent>();
	captainTransform.Translation = glm::vec3(x, y, 0.0f);

	auto& captainSprite = captain.AddComponent<SpriteComponent>();
	captainSprite.Texture = ResourceManager::GetTexture("assets/textures/entities/enemy_captain/Enemy_Captain_Idle.png");

	int enemyCount = rand() % 4 + 1;

	for (int i = 0; i < enemyCount; i++)
	{
		auto knight = m_GameObjectManager->Create("Enemy Knight");

		knight.AddComponent<NativeScriptComponent>().Bind<EnemyKnight>();
		EnemyKnight* script = dynamic_cast<EnemyKnight*>(knight.GetComponent<NativeScriptComponent>().Instance);
		script->SetLeader(captain);

		auto& transform = knight.GetComponent<TransformComponent>();
		transform.Translation = glm::vec3(x, y, 0.0f);

		auto& sprite = knight.AddComponent<SpriteComponent>();
		sprite.Texture = ResourceManager::GetTexture("assets/textures/entities/enemy_knight/Enemy_Knight_Idle.png");
	}
}

void Game::SpawnShip()
{
	PC_PROFILE_FUNCTION();

	auto ship = m_GameObjectManager->Create("Enemy Ship");
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
	PC_PROFILE_FUNCTION();

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
