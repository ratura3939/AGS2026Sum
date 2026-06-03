#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyDeathState.h"

EnemyDeathState::EnemyDeathState(void)
{
}

EnemyDeathState::~EnemyDeathState(void)
{
}

void EnemyDeathState::Enter(EnemyBase& _enemy)
{
}

void EnemyDeathState::Update(EnemyBase& _enemy)
{
	//死亡してからの時間が一定以上ならグループから離れる
	//if (deathTime_ > DEATH_TIME) _enemy.;
	//else deathTime_ += SceneManager::GetInstance().GetDeltaTime();
}

void EnemyDeathState::Exit(EnemyBase& _enemy)
{
}
