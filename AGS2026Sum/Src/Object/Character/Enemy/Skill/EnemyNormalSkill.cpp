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

void EnemyNormalSkill::ReadyEnter(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;
}

const bool EnemyNormalSkill::ReadyUpdate(EnemyBase& _owner)
{
	//準備が終わったらtrue
	if (attackCnt_ > ATTACK_READY_TIME)return true;
	else attackCnt_ += SceneManager::GetInstance().GetDeltaTime();

	return false;
}

void EnemyNormalSkill::ReadyExit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;
}

void EnemyNormalSkill::Enter(EnemyBase& _owner)
{
	//攻撃コライダの有効化
	_owner.EnableAttack();

	//攻撃座標設定
	_owner.SetAttackPos(ATTACK_LOCAL_POS);

	//アニメーション
	_owner.PlayAnim(L"Attack");

	//初期化
	attackCnt_ = 0.0f;
}

const bool EnemyNormalSkill::Update(EnemyBase& _owner)
{
	//攻撃が終わったらtrue
	if (attackCnt_ > ATTACK_TIME)return true;
	else attackCnt_ += SceneManager::GetInstance().GetDeltaTime();

	return false;
}

void EnemyNormalSkill::Exit(EnemyBase& _owner)
{
	//攻撃コライダの無効化
	_owner.DisableAttack();

	//初期化
	attackCnt_ = 0.0f;
}