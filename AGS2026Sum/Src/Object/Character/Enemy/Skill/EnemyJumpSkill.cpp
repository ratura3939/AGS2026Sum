#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/GameSystem/AttackManager.h"
#include "../../../../Manager/Decoration/SoundManager.h"
#include "../../../../Manager/Decoration/EffectManager.h"
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

	//現在状態
	state_ = SKILL_STATE::READY;

	//アニメーション
	_owner.PlayAnim(L"Jump");

	//エフェクト
	EffectManager::GetInstance().Play(_owner.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_AURA, _owner.GetPos(), _owner.GetQua(), AURA_SCALE);

	//特殊スキル
	_owner.SetIsElementSkill(true);

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::ENEMY_CHARGE_SE);
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

	//エフェクトストップ
	EffectManager::GetInstance().StopAll();

	//終了
	_owner.SetIsElementSkill(false);
}

void EnemyJumpSkill::Enter(EnemyBase& _owner)
{
	//現在状態
	state_ = SKILL_STATE::ACTIVE;

	//攻撃コライダの有効化
	_owner.EnableAttack();

	//攻撃マネージャーに教える
	_owner.SetAttackCollider(AttackManager::ATTACK_TYPE::E_JUMP);

	//攻撃座標設定
	_owner.SetAttackPos(ATTACK_LOCAL_POS);

	//攻撃範囲設定
	_owner.SetAttackRadius(RADIUS);

	//エフェクト
	EffectManager::GetInstance().Play(_owner.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_LANDING, _owner.GetPos(), _owner.GetQua(), EFF_SCALE);

	//初期化
	attackCnt_ = 0.0f;

	//特殊スキル
	_owner.SetIsElementSkill(true);

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::ENEMY_JUMP_SKILL_SE);
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

	//エフェクト
	//EffectManager::GetInstance().StopAll();

	//終了
	_owner.SetIsElementSkill(false);
}

void EnemyJumpSkill::EndEnter(EnemyBase& _owner)
{
	//現在状態
	state_ = SKILL_STATE::END;

	//攻撃コライダの無効化
	_owner.DisableAttack();

	//攻撃マネージャーから破棄
	_owner.RemoveAttackCollider();

	//初期化
	attackCnt_ = 0.0f;

	//終了
	_owner.SetIsElementSkill(false);
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

	//エフェクト
	//EffectManager::GetInstance().Stop(_owner.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_LANDING);
}
