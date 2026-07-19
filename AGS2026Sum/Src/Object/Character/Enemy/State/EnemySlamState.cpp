#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyKnockDownState.h"
#include "EnemyDeathState.h"
#include "EnemySlamState.h"

EnemySlamState::EnemySlamState(const VECTOR& _vec)
	: EnemyDamageState(_vec)
{
}

EnemySlamState::~EnemySlamState(void)
{
}

void EnemySlamState::Enter(EnemyBase& _enemy)
{
	//移動時間の初期化
	damageMoveTime_ = 0.0f;

	//ダメージアニメーション
	_enemy.PlayAnim(L"BlowSecondHalf", ANIM_SPEED);

	//移動量更新
	_enemy.SetMovePow(VScale(moveVec_, SPEED));

	//移動更新
	_enemy.Update();
}

void EnemySlamState::Update(EnemyBase& _enemy)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//移動時間が一定以上ならノックダウン状態に遷移
	if (damageMoveTime_ > DAMAGE_MOVE_TIME_MAX)
	{
		//生きているならノックダウンに移行
		if (_enemy.IsAlive())_enemy.ChangeState(std::make_unique<EnemyKnockDownState>());

		//死亡しているなら死亡状態に移行
		else _enemy.ChangeState(std::make_unique<EnemyDeathState>());
		
		return;
	}
	else
	{
		//移動時間の更新
		damageMoveTime_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

		//アニメーションが終了していたら終了アニメーションに遷移
		_enemy.PlayNoBlendAnimIsFinish(L"BlowEnd", ANIM_SPEED);
	}

	//移動方向に後ろを向きながら移動
	_enemy.BackMove();
}

void EnemySlamState::Exit(EnemyBase& _enemy)
{
	//ダメージアニメーション
	_enemy.PlayAnim(L"BlowEnd", ANIM_SPEED);
}