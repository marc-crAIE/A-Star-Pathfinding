#include "Game.h"

#include "Entity/Knight.h"
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

	cc.Camera.SetOrthographic(30.0f, -1.0f, 1.0f);

	for (int i = 0; i < 10; i++)
		SpawnKnight(3, 3);
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

	Renderer2D::BeginScene(m_Camera.GetComponent<CameraComponent>().Camera);
	m_World->OnRender();
	if (m_ShowNodeMap)
		m_NodeMap->OnRender();

	Renderer2D::EndScene();
}

void Game::SpawnKnight(float x, float y)
{
	auto knight = m_ActiveScene->CreateGameObject("Knight");
	knight.AddComponent<NativeScriptComponent>().Bind<Knight>();

	auto& transform = knight.GetComponent<TransformComponent>();
	transform.Translation = glm::vec3(x, y, 0.0f);

	auto& knightSprite = knight.AddComponent<SpriteComponent>();
	knightSprite.Texture = ResourceManager::GetTexture("assets/textures/knight/Knight_Idle.png");
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
