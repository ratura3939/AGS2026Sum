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
	_enemy.PlayNoBlendAnim(L"Launch", ANIM_SPEED);

	//移動量更新
	_enemy.SetMovePow(VScale(moveVec_, SPEED));

	//移動更新
	_enemy.Update();
}

void EnemyLaunchState::Update(EnemyBase& _enemy)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//移動時間が一定以上ならノックダウン状態に遷移
	if (damageMoveTime_ > DAMAGE_MOVE_TIME_MAX) _enemy.ChangeState(std::make_unique<EnemyKnockDownState>());
	else damageMoveTime_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	//移動方向に後ろを向きながら移動
	_enemy.Move();
}

void EnemyLaunchState::Exit(EnemyBase& _enemy)
{
	//ダメージアニメーション
	_enemy.PlayAnim(L"LaunchDown", ANIM_SPEED);
}
