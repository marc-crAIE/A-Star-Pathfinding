#include "EnemyKnight.h"

#include "../../Game.h"
#include "../../AI/Pathfinding/AStar.h"
#include "../../Utils/Utils.h"

#pragma region State Machine

#pragma region States

class AttackState : public State
{
public:
	void OnUpdate(GameObject gameObject, Timestep ts) override
	{
		EnemyKnight* knight = dynamic_cast<EnemyKnight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);
		if (knight)
		{
			GameObject attackTarget = knight->GetAttackTarget();
			if (Game::GetGOmanager()->Exists(attackTarget))
			{
				auto& transform = gameObject.GetComponent<TransformComponent>();
				auto& attackTargetTransform = attackTarget.GetComponent<TransformComponent>();
				if (glm::distance(transform.Translation, attackTargetTransform.Translation) <= knight->GetAttackRange())
				{
					auto* targetScript = dynamic_cast<AttackableEntity*>(attackTarget.GetComponent<NativeScriptComponent>().Instance);
					if (targetScript)
						knight->Attack(targetScript, knight->GetDamageAmount(), ts);
				}

				if (!knight->GetPathAlgo()->HasPath())
				{
					glm::vec2 targetPos = Utils::ScreenPosToWorldPos(attackTargetTransform.Translation);

					NodeMap::Node* node = Game::GetNodeMap()->GetNode(targetPos.x, targetPos.y);
					if (!node)
						return;

					knight->SetDestination(node);
					if (!knight->GetPathAlgo()->HasPath())
						return;
				}

				knight->FollowPath(ts);
			}
		}
	}
};

#pragma endregion

#pragma region Transitions

class ObjectNearTransition : public Transition
{
public:
	ObjectNearTransition(Ref<State> targetState, const std::string& targetTag, float distance)
		: Transition(targetState), m_Tag(targetTag), m_Distance(distance)
	{
		m_ConditionFn = [&](GameObject gameObject, Timestep ts) -> bool {
			auto& transform = gameObject.GetComponent<TransformComponent>();

			GameObject foundTarget = {};
			float targetDistance = INT32_MAX;

			auto possibleTargets = Game::GetGOmanager()->GetGameObjectsFromTag(m_Tag);
			for (GameObject possibleTarget : possibleTargets)
			{
				auto& pTargetTransform = possibleTarget.GetComponent<TransformComponent>();
				float distance = glm::distance(transform.Translation, pTargetTransform.Translation);
				if (distance <= m_Distance && distance < targetDistance)
				{
					foundTarget = possibleTarget;
					targetDistance = distance;
				}
			}

			if (foundTarget != GameObject{})
			{
				EnemyKnight* knight = dynamic_cast<EnemyKnight*>(gameObject.GetComponent<NativeScriptComponent>().Instance);
				if (knight)
				{
					knight->SetAttackTarget(foundTarget);
					return true;
				}
			}
			return false;
		};
	}
private:
	std::string m_Tag;
	float m_Distance;
};

#pragma endregion

#pragma endregion

EnemyKnight::EnemyKnight(bool captain)
{
	m_Type = captain ? CAPTAIN : KNIGHT;
}

void EnemyKnight::OnCreate()
{
	PC_PROFILE_FUNCTION();

	// Behaviour tree is not being used
	m_BehaviourTree = nullptr;

	m_Speed = 3.0f;
	m_Castle = Game::GetGOmanager()->GetGameObjectFromTag("Castle");
	m_PathAlgo = CreateRef<Pathfinding::AStar>();

	Ref<WanderState> wanderState = CreateRef<WanderState>();
	Ref<FollowState> followState = CreateRef<FollowState>(m_Leader);
	Ref<AttackState> attackState = CreateRef<AttackState>();

	Ref<TargetFarTransition> targetFar = CreateRef<TargetFarTransition>(wanderState, m_Leader, 5.0f);
	Ref<ObjectNearTransition> knightNearCondition = CreateRef<ObjectNearTransition>(attackState, "Knight", 8.0f);

	followState->AddTransition(targetFar);

	wanderState->AddTransition(knightNearCondition);

	if (m_Type != CAPTAIN)
		m_State.RequestStateChange(followState);
	else
		m_State.RequestStateChange(wanderState);
}

void EnemyKnight::OnUpdate(Timestep ts)
{
	PC_PROFILE_FUNCTION();

	m_State.OnUpdate(GetGameObject(), ts);
}
