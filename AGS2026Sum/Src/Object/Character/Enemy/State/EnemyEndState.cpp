#include "../../../../pch.h"
#include "../../../../Utility/Utility.h"
#include "../../../../Manager/Generic/SceneManager.h"
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
}

void EnemyEndState::Update(EnemyBase& _enemy)
{
	//消えるまでの時間が一定以上なら終了状態に遷移
	if (fadeTime_ < FADE_TIME)fadeTime_ += SceneManager::GetInstance().GetDeltaTime();
	
	//落下はあるため移動処理を入れる
	_enemy.Move();
}

void EnemyEndState::Exit(EnemyBase& _enemy)
{
	_enemy.SubEventCount();
}

const ENEMY_STATE EnemyEndState::GetStateId(void) const
{
	//消えるまでの時間が一定以上なら終了状態に遷移
	return fadeTime_ < FADE_TIME ? ENEMY_STATE::FADE : ENEMY_STATE::END;
}
