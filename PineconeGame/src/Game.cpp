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

	m_World = CreateRef<World>();
	m_NodeMap = CreateRef<NodeMap>(m_World);
}

void Game::OnAttach()
{
	RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });

	m_ActiveScene = CreateRef<Scene>();

	m_Camera = m_ActiveScene->CreateGameObject("Camera");
	auto& cc = m_Camera.AddComponent<CameraComponent>();

	cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

	cc.Camera.SetOrthographic(30.0f, -1.0f, 1.0f);


	// TEMPORARY CODE
	m_Knight = m_ActiveScene->CreateGameObject("Knight");
	m_Knight.AddComponent<NativeScriptComponent>().Bind<Knight>();
	auto& knightSprite = m_Knight.AddComponent<SpriteComponent>();
	knightSprite.Texture = ResourceManager::GetTexture("assets/textures/knight/Knight_Idle.png");
}

void Game::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(Game::OnWindowResized));
}

void Game::OnUpdate(Timestep ts)
{
	Renderer2D::ResetStats();
	RenderCommand::Clear();

	if (Input::IsMouseButtonPressed(Mouse::ButtonLeft) || Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		if (!m_MousePressed)
		{
			m_MousePressed = true;

			/*glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0) + glm::vec2(0.5f, -0.5f);
			glm::vec2 mousePos = Utils::MouseToOpenGLCoords() + offset;
			if (mousePos.x >= 0 && mousePos.x < WORLD_WIDTH && mousePos.y >= 0 && mousePos.y < WORLD_HEIGHT)
			{
				PC_INFO("Tile: {0}, {1}", glm::floor(mousePos.x), glm::floor(mousePos.y));

				if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
					m_StartNode = m_NodeMap->GetNode(mousePos.x, WORLD_HEIGHT - mousePos.y);
				else
					m_EndNode = m_NodeMap->GetNode(mousePos.x, WORLD_HEIGHT - mousePos.y);
			}

			if (m_StartNode && m_EndNode)
			{
				PC_INFO("Find Path!");
				m_PathAlgo.Search(m_NodeMap, m_StartNode, m_EndNode);
				PC_INFO("Number of nodes in path: {0}", m_PathAlgo.GetPath().size());
			}*/
		}
	}
	else if (m_MousePressed)
		m_MousePressed = false;

	m_ActiveScene->OnUpdate(ts);

	Renderer2D::BeginScene(m_Camera.GetComponent<CameraComponent>().Camera);
	m_World->OnRender();
	//m_NodeMap->OnRender();

	glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0);
	if (m_StartNode)
	{
		glm::vec2 pos = m_StartNode->GetPosition() - offset;
		Renderer2D::DrawQuad(glm::vec3(pos, 1.0f), glm::vec2(0.75f), glm::vec4(1.0f, 1.0f, 0.1f, 1.0f));
	}
	if (m_EndNode)
	{
		glm::vec2 pos = m_EndNode->GetPosition() - offset;
		Renderer2D::DrawQuad(glm::vec3(pos, 1.0f), glm::vec2(0.75f), glm::vec4(1.0f, 0.6f, 0.1f, 1.0f));
	}

	if (m_PathAlgo.GetPath().size() > 1 && m_StartNode && m_EndNode)
	{
		glm::vec2 prevPos = glm::vec2(0.0f);
		for (int i = 0; i < m_PathAlgo.GetPath().size(); i++)
		{
			if (i != 0)
			{
				glm::vec2 pos = m_PathAlgo.GetPath()[i]->GetPosition();
				Renderer2D::DrawLine(glm::vec3(prevPos - offset, 1.0f), glm::vec3(pos - offset, 1.0f), glm::vec4(1.0f));
				prevPos = pos;
			}
			else
				prevPos = m_PathAlgo.GetPath()[i]->GetPosition();
		}
	}

	Renderer2D::EndScene();
}

bool Game::OnWindowResized(WindowResizeEvent& e)
{
	m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
	return false;
}
