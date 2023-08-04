#include "Knight.h"

#include <Pinecone.h>

#include "../AI/Pathfinding/AStar.h"
#include "../Utils/Utils.h"

using namespace Pinecone;

float Knight::m_MoveSpeed = 5.0f;

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

static BehaviourStatus IsWanderingCondition(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight)
		return knight->IsWandering() ? BH_SUCCESS : BH_FAILURE;

	return BH_FAILURE;
}

static BehaviourStatus IsNotWanderingCondition(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight)
		return knight->IsWandering() ? BH_FAILURE : BH_SUCCESS;

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
	bool success = knight->FollowPath(Knight::GetSpeed(), ts);
	return success ? BH_SUCCESS : BH_FAILURE;
}

#pragma endregion

void Knight::OnCreate()
{
	m_BehaviourTree =
		(new FallbackNode())->Add(
			(new SequenceNode())->Add(
				new FuncAction(HasPathCondition))->Add(
				new FuncAction(IsNotWanderingCondition))->Add(
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
								new SetSpriteColorAction(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)))
						)->Add(
							(new SequenceNode())->Add(
								new SetSpriteColorAction(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)))->Add(
								new WanderAction(2.0f))
						))
				)->Add(
					(new SequenceNode())->Add(
						new SetSpriteColorAction(glm::vec4(1.0f)))->Add(
						new WanderAction(2.0f))
				)
			)
		);

	m_PathAlgo = CreateRef<Pathfinding::AStar>();
}

void Knight::OnUpdate(Timestep ts)
{
	m_BehaviourTree->Tick(GetGameObject(), ts);
}
