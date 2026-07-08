#include "../../../../pch.h"
#include "../../../../Utility/Utility.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyNormalState.h"
#include "EnemyDeathState.h"
#include "EnemyKnockDownState.h"

EnemyKnockDownState::EnemyKnockDownState(void)
	: knockDownTime_(0.0f)
{
}

EnemyKnockDownState::~EnemyKnockDownState(void)
{
}

void EnemyKnockDownState::Enter(EnemyBase& _enemy)
{
	//倒れ時間の初期化
	knockDownTime_ = 0.0f;

	//移動量をなくす
	_enemy.SetMovePow(Utility::VECTOR_ZERO);
}

void EnemyKnockDownState::Update(EnemyBase& _enemy)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();
	
	//ダウン時間が一定以上なら通常状態に遷移
	if (knockDownTime_ > KNOCK_DOWN_TIME_MAX)
	{
		//生きているなら通常に移行
		if (_enemy.IsAlive())_enemy.ChangeState(std::make_unique<EnemyNormalState>());

		//死亡しているなら死亡状態に移行
		else _enemy.ChangeState(std::make_unique<EnemyDeathState>());

		return;
	}

	//ダウン時間が一定未満ならカウントアップ
	else knockDownTime_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	//落下はあるため移動処理を入れる
	_enemy.Move();
}

void EnemyKnockDownState::Exit(EnemyBase& _enemy)
{
}