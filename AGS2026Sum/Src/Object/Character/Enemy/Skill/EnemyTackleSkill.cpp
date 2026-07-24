#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/GameSystem/AttackManager.h"
#include "../../../../Manager/Decoration/SoundManager.h"
#include "../../../../Manager/Decoration/EffectManager.h"
#include "../EnemyBase.h"
#include "EnemyTackleSkill.h"

EnemyTackleSkill::EnemyTackleSkill(void)
	:effHandle_(-1)
{
}

EnemyTackleSkill::~EnemyTackleSkill(void)
{
}

void EnemyTackleSkill::ReadyEnter(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;

	//アニメーション
	_owner.PlayNoBlendAnim(L"Tackle", TACKLE_PRE_ANIM_SPEED);

	//エフェクト
	EffectManager::GetInstance().Play(_owner.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_AURA, _owner.GetPos(), _owner.GetQua(), AURA_SCALE);

	//特殊スキル
	_owner.SetIsElementSkill(true);

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::ENEMY_CHARGE_SE);
}

const bool EnemyTackleSkill::ReadyUpdate(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//準備が終わったらtrue
	if (attackCnt_ > ATTACK_READY_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());
	
	//対象にベクトルを合わせる
	_owner.UpdateMovePow();
	_owner.Rotation();

	return false;
}

void EnemyTackleSkill::ReadyExit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;

	//アニメーション
	_owner.PlayNoBlendAnim(L"Idle");

	//エフェクトストップ
	EffectManager::GetInstance().StopAll();

	//特殊スキル
	_owner.SetIsElementSkill(false);
}

void EnemyTackleSkill::Enter(EnemyBase& _owner)
{
	//攻撃コライダの有効化
	_owner.EnableAttack();

	//攻撃マネージャーに教える
	_owner.SetAttackCollider(AttackManager::ATTACK_TYPE::E_TACKLE);

	//攻撃座標設定
	_owner.SetAttackPos(ATTACK_LOCAL_POS);

	//攻撃範囲設定
	_owner.SetAttackRadius(RADIUS);

	//アニメーション
	_owner.PlayNoBlendAnim(L"Tackle", TACKLE_ANIM_SPEED);

	//移動速度設定
	_owner.SetSpeed(TACKLE_SPEED);

	//初回のみの移動量更新
	_owner.UpdateMovePow();

	//エフェクト
	effHandle_ = EffectManager::GetInstance().Play(_owner.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_TACKLE, _owner.GetPos(), _owner.GetQua().Mult(Quaternion::Euler({ 0.0f,Utility::Deg2RadF(180.0f),0.0f })), EFF_SCALE);

	//初期化
	attackCnt_ = 0.0f;

	//特殊スキル
	_owner.SetIsElementSkill(true);

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::ENEMY_TACKLE_SKILL_SE);
}

const bool EnemyTackleSkill::Update(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//攻撃が終わったらtrue
	if (attackCnt_ > ATTACK_TIME)
	{
		return true;
	}
	else
	{
		//カウンタ更新
		attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

		//移動
		_owner.Move();

		//攻撃座標も動かす
		_owner.SetAttackPos(ATTACK_LOCAL_POS);

		//エフェクト
		EffectManager::GetInstance().SyncEffect(effHandle_, _owner.GetPos(), _owner.GetQua().Mult(Quaternion::Euler({0.0f,Utility::Deg2RadF(180.0f),0.0f})), EFF_SCALE, 1.0f);
	}

	return false;
}

void EnemyTackleSkill::Exit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;

	//エフェクトストップ
	EffectManager::GetInstance().StopAll();

	//特殊スキル終了
	_owner.SetIsElementSkill(false);
}

void EnemyTackleSkill::EndEnter(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;

	//攻撃コライダの無効化
	_owner.DisableAttack();

	//攻撃マネージャーから破棄
	_owner.RemoveAttackCollider();

	//移動速度設定
	_owner.SetSpeed(0.0f);
}

const bool EnemyTackleSkill::EndUpdate(EnemyBase& _owner)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//攻撃が終わったらtrue
	if (attackCnt_ > ATTACK_END_TIME)return true;
	else attackCnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	return false;
}

void EnemyTackleSkill::EndExit(EnemyBase& _owner)
{
	//初期化
	attackCnt_ = 0.0f;

	//エフェクトストップ
	//EffectManager::GetInstance().StopAll();
}
