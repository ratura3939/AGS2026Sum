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
