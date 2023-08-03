#include "Knight.h"

#include <Pinecone.h>

#include "../AI/Pathfinding/AStar.h"
#include "../Utils/Utils.h"

using namespace Pinecone;

float Knight::m_MoveSpeed = 3.0f;

#pragma region Behaviours

static BehaviourStatus IsSelectedCondition(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight && knight->IsSelected())
		return BH_SUCCESS;

	return BH_FAILURE;
}

static BehaviourStatus SelectAction(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight)
	{
		knight->Select();
		return BH_SUCCESS;
	}

	return BH_FAILURE;
}

static BehaviourStatus HasPathCondition(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight && knight->GetPathAlgo()->HasPath())
		return BH_SUCCESS;

	return BH_FAILURE;
}

static BehaviourStatus UnselectAction(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight)
	{
		knight->Select(false);
		return BH_SUCCESS;
	}

	return BH_FAILURE;
}

static BehaviourStatus SetDestinationAction(GameObject gameObject, Timestep ts)
{
	glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0) + glm::vec2(0.5f, -0.5f);
	glm::vec2 mousePos = Utils::MouseToOpenGLCoords() + offset;

	if (mousePos.x >= 0 && mousePos.x < WORLD_WIDTH && mousePos.y >= 0 && mousePos.y < WORLD_HEIGHT)
	{
		Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);
		knight->SetDestination(Game::GetNodeMap()->GetNode(mousePos.x, WORLD_HEIGHT - mousePos.y));
		knight->Select(false);
		return BH_SUCCESS;
	}
	return BH_FAILURE;
}

static BehaviourStatus GotoDestinationAction(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight->GetPathAlgo()->HasPath())
	{
		glm::vec2 offset = glm::vec2(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0);

		auto& transform = knight->GetComponent<TransformComponent>();
		auto knightPos = glm::vec2(knight->GetComponent<TransformComponent>().Translation) + offset;
		auto path = knight->GetPathAlgo();

		NodeMap::Node* currNode = path->GetCurrentPathNode();
		float dist = glm::distance(knightPos, currNode->GetPosition());
		glm::vec2 dir = glm::normalize(currNode->GetPosition() - knightPos);

		if ((dist - (Knight::GetSpeed() * ts)) > 0)
		{
			transform.Translation += glm::vec3(Knight::GetSpeed() * dir * (float)ts, 0.0f);
		}
		else
		{
			if (currNode == path->GetPath()[path->GetPath().size() - 1])
			{
				path->ResetPath();
				return BH_SUCCESS;
			}

			currNode = path->GetNextPathNode();
		}

		return BH_SUCCESS;
	}
	return BH_FAILURE;
}

#pragma endregion

void Knight::OnCreate()
{
	m_BehaviourTree =
		(new FallbackNode())->Add(
			(new SequenceNode())->Add(
				new FuncAction(HasPathCondition))->Add(
				new FuncAction(GotoDestinationAction))
		)->Add(
			(new FallbackNode())->Add(
				(new SequenceNode())->Add(
					new FuncAction(IsSelectedCondition))->Add(
					(new FallbackNode())->Add(
						(new SequenceNode())->Add(
							new MouseButtonReleaseCondition(Mouse::ButtonLeft))->Add(
							new FuncAction(SetDestinationAction))->Add(
							new SetSpriteColorAction(glm::vec4(1.0f)))
					)->Add(
						(new SequenceNode())->Add(
							new UntilAction(3.0f))->Add(
							new FuncAction(UnselectAction))
					))
			)->Add(
					(new FallbackNode())->Add(
						(new SequenceNode())->Add(
							new MouseNearCondition(2))->Add(
							(new FallbackNode())->Add(
								(new SequenceNode())->Add(
									new MouseButtonReleaseCondition(Mouse::ButtonLeft))->Add(
									new FuncAction(SelectAction))->Add(
									new SetSpriteColorAction(glm::vec4(0.2f, 1.0f, 0.3f, 1.0f)))
							)->Add(
								(new SequenceNode())->Add(
									new SetSpriteColorAction(glm::vec4(1.0f)))
							))
					)->Add(
						(new SequenceNode())->Add(
							new SetSpriteColorAction(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)))
					)
			)
		);

	m_PathAlgo = CreateRef<Pathfinding::AStar>();
}

void Knight::OnUpdate(Timestep ts)
{
	m_BehaviourTree->Tick(GetGameObject(), ts);

	auto& transform = GetComponent<TransformComponent>();
	if (Input::IsKeyPressed(Key::W))
	{
		transform.Translation.x += 1.0f * ts;
	}
}
