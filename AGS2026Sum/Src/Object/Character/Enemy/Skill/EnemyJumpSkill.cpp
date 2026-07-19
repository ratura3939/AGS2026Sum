#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/GameSystem/AttackManager.h"
#include "../EnemyBase.h"
#include "EnemyJumpSkill.h"

EnemyJumpSkill::EnemyJumpSkill(void)
{
}

EnemyJumpSkill::~EnemyJumpSkill(void)
{
}

void EnemyJumpSkill::ReadyEnter(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;

	//アニメーション
	_owner.PlayAnim(L"Jump");
}

const bool EnemyJumpSkill::ReadyUpdate(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//準備が終わったらtrue
	if (attackCnt_ > ATTACK_READY_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	return false;
}

void EnemyJumpSkill::ReadyExit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;
}

void EnemyJumpSkill::Enter(EnemyBase& _owner)
{
	//攻撃コライダの有効化
	_owner.EnableAttack();

	//攻撃マネージャーに教える
	_owner.SetAttackCollider(AttackManager::ATTACK_TYPE::E_JUMP);

	//攻撃座標設定
	_owner.SetAttackPos(ATTACK_LOCAL_POS);

	//攻撃範囲設定
	_owner.SetAttackRadius(RADIUS);

	//初期化
	attackCnt_ = 0.0f;
}

const bool EnemyJumpSkill::Update(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//準備が終わったらtrue
	if (attackCnt_ > ATTACK_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	return false;
}

void EnemyJumpSkill::Exit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;
}

void EnemyJumpSkill::EndEnter(EnemyBase& _owner)
{
	//攻撃コライダの無効化
	_owner.DisableAttack();

	//攻撃マネージャーから破棄
	_owner.RemoveAttackCollider();

	//初期化
	attackCnt_ = 0.0f;
}

const bool EnemyJumpSkill::EndUpdate(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//攻撃が終わったらtrue
	if (attackCnt_ > ATTACK_END_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	return false;
}

void EnemyJumpSkill::EndExit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;
}
