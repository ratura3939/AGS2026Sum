#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/GameSystem/AttackManager.h"
#include "../../../../Manager/Decoration/SoundManager.h"
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

	//アニメーション
	_owner.PlayAnim(L"Attack");
}

const bool EnemyNormalSkill::ReadyUpdate(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//準備が終わったらtrue
	if (attackCnt_ > ATTACK_READY_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

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

	//攻撃マネージャーに教える
	_owner.SetAttackCollider(AttackManager::ATTACK_TYPE::E_NORMAL);

	//攻撃座標設定
	_owner.SetAttackPos(ATTACK_LOCAL_POS);

	//攻撃範囲設定
	_owner.SetAttackRadius(RADIUS);

	//初期化
	attackCnt_ = 0.0f;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::ENEMY_NORMAL_SKILL_SE);
}

const bool EnemyNormalSkill::Update(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//攻撃が終わったらtrue
	if (attackCnt_ > ATTACK_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	return false;
}

void EnemyNormalSkill::Exit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;
}

void EnemyNormalSkill::EndEnter(EnemyBase& _owner)
{
	//攻撃コライダの無効化
	_owner.DisableAttack();

	//攻撃マネージャーから破棄
	_owner.RemoveAttackCollider();

	//初期化
	attackCnt_ = 0.0f;
}

const bool EnemyNormalSkill::EndUpdate(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//攻撃が終わったらtrue
	if (attackCnt_ > ATTACK_END_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	return false;
}

void EnemyNormalSkill::EndExit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;
}
