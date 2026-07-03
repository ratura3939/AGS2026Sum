#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyNormalState.h"
#include "EnemyDeathState.h"
#include "EnemyStaggerState.h"

EnemyStaggerState::EnemyStaggerState(const VECTOR& _vec)
	: EnemyDamageState(_vec)
{
}

EnemyStaggerState::~EnemyStaggerState(void)
{
}

void EnemyStaggerState::Enter(EnemyBase& _enemy)
{
	//移動時間の初期化
	damageMoveTime_ = 0.0f;

	//ダメージアニメーション
	_enemy.PlayNoBlendAnim(L"Stagger", ANIM_SPEED);

	//移動量更新
	_enemy.SetMovePow(VScale(moveVec_, SPEED));

	//移動更新
	_enemy.Update();
}

void EnemyStaggerState::Update(EnemyBase& _enemy)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//移動時間が一定以上ならノックダウン状態に遷移
	if (damageMoveTime_ > DAMAGE_MOVE_TIME_MAX)
	{
		if (_enemy.IsAlive())_enemy.ChangeState(std::make_unique<EnemyNormalState>());
		else _enemy.ChangeState(std::make_unique<EnemyDeathState>());
		return;
	}
	else damageMoveTime_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	//移動
	_enemy.Move();
}

void EnemyStaggerState::Exit(EnemyBase& _enemy)
{
}
