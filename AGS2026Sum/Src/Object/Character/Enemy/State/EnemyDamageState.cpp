#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyNormalState.h"
#include "EnemyDamageState.h"

EnemyDamageState::EnemyDamageState(void)
	: damageMoveTime_(0.0f)
{
}

EnemyDamageState::~EnemyDamageState(void)
{
}

void EnemyDamageState::Enter(EnemyBase& _enemy)
{
	//移動時間の初期化
	damageMoveTime_ = 0.0f;

	//ダメージアニメーション
	_enemy.PlayAnim(L"Blow");
}

void EnemyDamageState::Update(EnemyBase& _enemy)
{
	//移動時間が一定以上なら通常状態に遷移
	if (damageMoveTime_ > DAMAGE_MOVE_TIME_MAX) _enemy.ChangeState(std::make_unique<EnemyNormalState>());
	else damageMoveTime_ += SceneManager::GetInstance().GetDeltaTime();

	//移動更新
	_enemy.BackMove();
}

void EnemyDamageState::Exit(EnemyBase& _enemy)
{
}
