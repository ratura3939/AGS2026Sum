#include "../../../../pch.h"
#include "../../../../Utility/Utility.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyEndState.h"
#include "EnemyDeathState.h"

EnemyDeathState::EnemyDeathState(void)
	: deathTime_(0.0f)
{
}

EnemyDeathState::~EnemyDeathState(void)
{
}

void EnemyDeathState::Enter(EnemyBase& _enemy)
{
	//死亡してからの時間を初期化
	deathTime_ = 0.0f;

	//移動量をなくす
	_enemy.SetMovePow(Utility::VECTOR_ZERO);
}

void EnemyDeathState::Update(EnemyBase& _enemy)
{
	//死亡してからの時間が一定以上ならグループから離れる
	if (deathTime_ > DEATH_TIME)
	{
		//終了状態に移行
		_enemy.ChangeState(std::make_unique<EnemyEndState>());
	}
	else
	{
		//死亡してからの時間を更新
		deathTime_ += SceneManager::GetInstance().GetDeltaTime();
	}

	//落下はあるため移動処理を入れる
	_enemy.Move();
}

void EnemyDeathState::Exit(EnemyBase& _enemy)
{
}