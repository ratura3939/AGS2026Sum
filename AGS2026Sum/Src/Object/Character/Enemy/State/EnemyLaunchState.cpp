#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyKnockDownState.h"
#include "EnemyLaunchState.h"

EnemyLaunchState::EnemyLaunchState(const VECTOR& _vec)
	: EnemyDamageState(_vec)
{
}

EnemyLaunchState::~EnemyLaunchState(void)
{
}

void EnemyLaunchState::Enter(EnemyBase& _enemy)
{
	//移動時間の初期化
	damageMoveTime_ = 0.0f;

	//ダメージアニメーション
	_enemy.PlayAnim(L"Launch", ANIM_SPEED);

	//移動量更新
	_enemy.SetMovePow(VScale(moveVec_, SPEED));

	//移動更新
	_enemy.Update();

	//通常状態
	_enemy.ChangeAction(ENEMY_ACTION::STAY);
}

void EnemyLaunchState::Update(EnemyBase& _enemy)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//移動時間が一定以上ならノックダウン状態に遷移
	if (damageMoveTime_ > DAMAGE_MOVE_TIME_MAX) _enemy.ChangeState(std::make_unique<EnemyKnockDownState>());
	else damageMoveTime_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	//移動方向に後ろを向きながら移動
	_enemy.BackMove();
}

void EnemyLaunchState::Exit(EnemyBase& _enemy)
{
	//ダメージアニメーション
	_enemy.PlayAnim(L"BlowEnd", ANIM_SPEED);
}
