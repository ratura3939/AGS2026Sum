#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyNormalSkill.h"

EnemyNormalSkill::EnemyNormalSkill(void)
{
}

EnemyNormalSkill::~EnemyNormalSkill(void)
{
}

void EnemyNormalSkill::Enter(EnemyBase& _owner)
{
	//攻撃コライダの有効化
	_owner.EnableAttack();
}

void EnemyNormalSkill::Update(EnemyBase& _owner)
{
	//攻撃が終わったら待機状態に移行させる
	if (IsEnd())_owner.ChangeAction(ENEMY_ACTION::STAY);
	else attackCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void EnemyNormalSkill::Exit(EnemyBase& _owner)
{
	//攻撃コライダの無効化
	_owner.DisableAttack();
}

const bool EnemyNormalSkill::IsEnd(void) const
{
	return false;
}
