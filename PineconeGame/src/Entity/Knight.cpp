#include "Knight.h"

#include "../AI/Pathfinding/AStar.h"

#pragma region Behaviours

static BehaviourStatus TestAction()
{
	PC_TRACE("Just a test!");
	return BH_SUCCESS;
}

#pragma endregion

void Knight::OnCreate()
{
	m_BehaviourTree =
		(new FallbackNode())->Add(
			(new SequenceNode())->Add(
				new MouseNearCondition(5))->Add(
				new SetSpriteColorAction(glm::vec4(1.0f))
				))->Add(
			(new SequenceNode())->Add(
				new SetSpriteColorAction(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)))->Add(
				new FuncAction(TestAction)));

	m_PathAlgo = Pathfinding::AStar();
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
