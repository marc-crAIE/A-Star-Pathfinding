#include "Knight.h"

#include "../AI/Pathfinding/AStar.h"
#include "../Utils/Utils.h"

#pragma region Behaviours

static BehaviourStatus IsSelectedCondition(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight && knight->IsSelected())
		return BH_SUCCESS;

	return BH_FAILURE;
}

static BehaviourStatus IsNotSelectedCondition(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight && !knight->IsSelected())
		return BH_SUCCESS;

	return BH_FAILURE;
}

static BehaviourStatus IsAttackingCondition(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight && knight->IsAttacking())
		return BH_SUCCESS;

	return BH_FAILURE;
}

static BehaviourStatus IsTargetFar(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight && knight->IsAttacking())
	{
		auto& transform = gameObject.GetComponent<TransformComponent>();
		// Check if the attack target does not exist, they are definitely far away in that case
		if (!Game::GetGOmanager()->Exists(knight->GetAttackTarget()))
			return BH_SUCCESS;

		auto& targetTransform = knight->GetAttackTarget().GetComponent<TransformComponent>();

		float distance = glm::distance(transform.Translation, targetTransform.Translation);

		if (distance >= 8.0f)
			return BH_SUCCESS;
	}

	return BH_FAILURE;
}

static BehaviourStatus AttackTargetAction(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);

	if (knight)
	{
		auto& transform = gameObject.GetComponent<TransformComponent>();
		GameObject attackTarget = knight->GetAttackTarget();
		auto& targetTransform = attackTarget.GetComponent<TransformComponent>();

		if (glm::distance(transform.Translation, targetTransform.Translation) <= knight->GetAttackRange())
		{
			auto* targetScript = dynamic_cast<AttackableEntity*>(attackTarget.GetComponent<NativeScriptComponent>().Instance);
			if (targetScript)
				knight->Attack(targetScript, knight->GetDamageAmount(), ts);
		}

		if (!knight->GetPathAlgo()->HasPath())
		{
			glm::vec2 targetPos = Utils::ScreenPosToWorldPos(targetTransform.Translation);

			NodeMap::Node* node = Game::GetNodeMap()->GetNode(targetPos.x, targetPos.y);
			if (!node)
				return BH_FAILURE;

			knight->SetDestination(node);
			if (!knight->GetPathAlgo()->HasPath())
				return BH_FAILURE;
		}

		knight->FollowPath(ts);
		return BH_SUCCESS;
	}

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
	bool success = knight->FollowPath(ts);
	return success ? BH_SUCCESS : BH_FAILURE;
}

static BehaviourStatus SetAttackTargetAction(GameObject gameObject, Timestep ts)
{
	auto& transform = gameObject.GetComponent<TransformComponent>();

	GameObject target;
	float distance = FLT_MAX;

	auto targets = Game::GetGOmanager()->GetGameObjectsFromTag("Enemy Knight");
	for (GameObject t : targets)
	{
		auto& targetTransform = t.GetComponent<TransformComponent>();
		float d = glm::distance(transform.Translation, targetTransform.Translation);

		if (d < distance)
		{
			distance = d;
			target = t;
		}
	}

	targets = Game::GetGOmanager()->GetGameObjectsFromTag("Enemy Captain");
	for (GameObject t : targets)
	{
		auto& targetTransform = t.GetComponent<TransformComponent>();
		float d = glm::distance(transform.Translation, targetTransform.Translation);

		if (d < distance)
		{
			distance = d;
			target = t;
		}
	}

	if (distance == FLT_MAX)
		return BH_FAILURE;

	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);
	if (!knight)
		return BH_FAILURE;

	knight->SetAttackTarget(target);
	return BH_SUCCESS;
}

static BehaviourStatus ResetAttackTargetAction(GameObject gameObject, Timestep ts)
{
	Knight* knight = dynamic_cast<Knight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);
	if (!knight)
		return BH_FAILURE;

	knight->ResetAttackTarget();
	return BH_SUCCESS;
}

#pragma endregion

void Knight::OnCreate()
{
	PC_PROFILE_FUNCTION();

	m_Castle = Game::GetGOmanager()->GetGameObjectFromTag("Castle");
	m_Speed = 3.0f;

	m_BehaviourTree =
		new FallbackNode({
			new SequenceNode({
				new FuncAction(IsAttackingCondition),
				new FallbackNode({
					new SequenceNode({
						new FuncAction(IsTargetFar),
						new FuncAction(ResetAttackTargetAction)
					}),
					new FuncAction(AttackTargetAction)
				})
			}),
			new SequenceNode({
				new FuncAction(IsNotSelectedCondition),
				new ParallelNode(1, {
					new ObjectNearCondition("Enemy Knight", m_EnemyDetectionDistance),
					new ObjectNearCondition("Enemy Captain", m_EnemyDetectionDistance)
				}),
				new FuncAction(SetAttackTargetAction),
				new SetSpriteColorAction(glm::vec4(1.0f))
			}),
			new SequenceNode({
				new FuncAction(HasPathCondition),
				new FuncAction(IsNotWanderingCondition),
				new FuncAction(GotoDestinationAction)
			}),
			new FallbackNode({
				new SequenceNode({
					new FuncAction(IsSelectedCondition),
					new FallbackNode({
						new SequenceNode({
							new MouseButtonReleaseCondition(Mouse::ButtonLeft),
							new FuncAction(SetDestinationAction),
							new SetSpriteColorAction(glm::vec4(1.0f))
						}),
						new SequenceNode({
							new UntilAction(3.0f),
							new FuncAction(UnselectAction)
						})
					})
				}),
				new FallbackNode({
					new SequenceNode({
						new MouseNearCondition(2.0f),
						new FallbackNode({
							new SequenceNode({
								new MouseButtonReleaseCondition(Mouse::ButtonLeft),
								new FuncAction(SelectAction),
								new SetSpriteColorAction(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f))
							}),
							new SequenceNode({
								new SetSpriteColorAction(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)),
								new WanderAroundObjectAction(m_Castle)
							})
						})
					}),
					new SequenceNode({
						new SetSpriteColorAction(glm::vec4(1.0f)),
						new WanderAroundObjectAction(m_Castle)
					})
				})
			})
		});
	
	m_PathAlgo = CreateRef<Pathfinding::AStar>();
}

void Knight::OnUpdate(Timestep ts)
{
	PC_PROFILE_FUNCTION();

	m_BehaviourTree->Tick(GetGameObject(), ts);
}
