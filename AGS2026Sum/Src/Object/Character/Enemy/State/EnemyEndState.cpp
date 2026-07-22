#include "../../../../pch.h"
#include "../../../../Utility/Utility.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/Decoration/EffectManager.h"
#include "../EnemyBase.h"
#include "EnemyEndState.h"

EnemyEndState::EnemyEndState(void)
	: fadeTime_(0.0f)
{
}

EnemyEndState::~EnemyEndState(void)
{
}

void EnemyEndState::Enter(EnemyBase& _enemy)
{
	//消えるまでの時間を初期化
	fadeTime_ = 0.0f;

	//移動量をなくす
	_enemy.SetMovePow(Utility::VECTOR_ZERO);

	//死亡アニメーション
	_enemy.PlayAnim(L"BlowEnd");

	//死亡エフェクト
	EffectManager::GetInstance().Play(_enemy.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_DEAD, _enemy.GetPos(), _enemy.GetQua(), 10.0f);
}

void EnemyEndState::Update(EnemyBase& _enemy)
{
	//消えるまでの時間が一定以上なら終了状態に遷移
	if (fadeTime_ > -1.0f)
	{
		fadeTime_ -= SceneManager::GetInstance().GetDeltaTime() / FADE_TIME;
		fadeTime_ = std::max(fadeTime_, -1.0f);
		_enemy.SetColor(FLOAT4(0.0f,0.0f,0.0f,fadeTime_));
	}

	//落下はあるため移動処理を入れる
	_enemy.Move();
}

void EnemyEndState::Exit(EnemyBase& _enemy)
{
	//イベントカウント減少
	_enemy.SubEventCount();

	//死亡エフェクト
	//EffectManager::GetInstance().Stop(_enemy.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_HIT);
}

const ENEMY_STATE EnemyEndState::GetStateId(void) const
{
	//消えるまでの時間が一定以上なら終了状態に遷移
	return fadeTime_ > -1.0f ? ENEMY_STATE::FADE : ENEMY_STATE::END;
}
