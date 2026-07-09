#include "../../../../pch.h"
#include "../EnemyBase.h"
#include "EnemyKnockDownState.h"
#include "EnemyDeathState.h"
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

	//接地判定の無効化
	_enemy.SetGrounding(false);

	//打ち上げの強さ
	_enemy.SetGravityPow(VGet(0.0f,LAUNCH_POW,0.0f));

	//ダメージアニメーション
	_enemy.PlayNoBlendAnim(L"Launch", ANIM_SPEED);

	//移動量更新
	_enemy.SetMovePow(VScale(moveVec_, SPEED));

	//移動更新
	_enemy.Update();
}

void EnemyLaunchState::Update(EnemyBase& _enemy)
{
	//重力反映
	VECTOR gravityPow = _enemy.GetGravityPow();

	//地面に着地したらノックダウン状態に遷移
	if (_enemy.IsGrounding() && gravityPow.y <= 0.0f)
	{
		//生きているならノックダウンに移行
		if (_enemy.IsAlive())_enemy.ChangeState(std::make_unique<EnemyKnockDownState>());

		//死亡しているなら死亡状態に移行
		else _enemy.ChangeState(std::make_unique<EnemyDeathState>());

		return;
	}

	//移動方向に移動
	_enemy.Move();
}

void EnemyLaunchState::Exit(EnemyBase& _enemy)
{
	//ダメージアニメーション
	_enemy.PlayAnim(L"LaunchDown", ANIM_SPEED);
}