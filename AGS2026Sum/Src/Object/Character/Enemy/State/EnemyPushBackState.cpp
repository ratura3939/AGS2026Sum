#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyNormalState.h"
#include "EnemyDeathState.h"
#include "EnemyPushBackState.h"

EnemyPushBackState::EnemyPushBackState(const VECTOR& _vec)
	: EnemyDamageState(_vec)
{
}

EnemyPushBackState::~EnemyPushBackState(void)
{
}

void EnemyPushBackState::Enter(EnemyBase& _enemy)
{
	//移動時間の初期化
	damageMoveTime_ = 0.0f;

	//ダメージアニメーション
	_enemy.PlayAnim(L"BlowFirstHalf", ANIM_SPEED);

	//移動量更新
	_enemy.SetMovePow(VScale(moveVec_, SPEED));

	//移動更新
	_enemy.Update();
}

void EnemyPushBackState::Update(EnemyBase& _enemy)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//移動時間が一定以上ならノックダウン状態に遷移
	if (damageMoveTime_ > DAMAGE_MOVE_TIME_MAX)
	{
		//生きているなら通常に移行
		if (_enemy.IsAlive())_enemy.ChangeState(std::make_unique<EnemyNormalState>());

		//死亡しているなら死亡状態に移行
		else _enemy.ChangeState(std::make_unique<EnemyDeathState>());

		return;
	}

	//移動時間が一定未満ならカウントアップ
	else damageMoveTime_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());


	//移動方向に後ろを向きながら移動
	_enemy.BackMove();
}

void EnemyPushBackState::Exit(EnemyBase& _enemy)
{
}