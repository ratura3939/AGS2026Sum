#include "../../../../pch.h"
#include "../EnemyBase.h"
#include "EnemyNormalState.h"

EnemyNormalState::EnemyNormalState(void) 
{
}

EnemyNormalState::~EnemyNormalState(void)
{
}

void EnemyNormalState::Enter(EnemyBase& _enemy)
{
	//当たり判定の有効化
	_enemy.EnableHitCollider();

	//アクションの初期化
	//_enemy.ChangeAction(ENEMY_ACTION::STAY);
}

void EnemyNormalState::Update(EnemyBase& _enemy)
{
	//思考の更新
	_enemy.UpdateBrain();

	//行動の更新
	_enemy.UpdateAction();
}

void EnemyNormalState::Exit(EnemyBase& _enemy)
{
}
