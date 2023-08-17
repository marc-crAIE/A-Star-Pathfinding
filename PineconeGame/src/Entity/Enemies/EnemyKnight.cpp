#include "EnemyKnight.h"

#include "../../Game.h"
#include "../../AI/Pathfinding/AStar.h"
#include "../../Utils/Utils.h"

EnemyKnight::EnemyKnight(bool captain)
{
	m_Type = captain ? CAPTAIN : KNIGHT;
}

void EnemyKnight::OnCreate()
{
	m_Speed = 3.0f;
	m_Castle = Game::GetScene()->GetGameObjectByTag("Castle");
	m_PathAlgo = CreateRef<Pathfinding::AStar>();

	Ref<WanderState> wanderState = CreateRef<WanderState>();
	Ref<FollowState> followState = CreateRef<FollowState>(m_Leader);

	Ref<TargetFarTransition> targetFar = CreateRef<TargetFarTransition>(wanderState, m_Leader, 5.0f);

	followState->AddTransition(targetFar);

	if (m_Type != CAPTAIN)
		m_State.RequestStateChange(followState);
	else
		m_State.RequestStateChange(wanderState);
}

void EnemyKnight::OnUpdate(Timestep ts)
{
	m_State.OnUpdate(GetGameObject(), ts);
}
