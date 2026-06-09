#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyNormalState.h"
#include "EnemyKnockDownState.h"

EnemyKnockDownState::EnemyKnockDownState(void)
{
}

EnemyKnockDownState::~EnemyKnockDownState(void)
{
}

void EnemyKnockDownState::Enter(EnemyBase& _enemy)
{
	knockDownTime_ = 0.0f;
}

void EnemyKnockDownState::Update(EnemyBase& _enemy)
{
	//ダウン時間が一定以上なら通常状態に遷移
	if (knockDownTime_ > KNOCK_DOWN_TIME_MAX) _enemy.ChangeState(std::make_unique<EnemyNormalState>());
	else knockDownTime_ += SceneManager::GetInstance().GetDeltaTime();
}

void EnemyKnockDownState::Exit(EnemyBase& _enemy)
{
}