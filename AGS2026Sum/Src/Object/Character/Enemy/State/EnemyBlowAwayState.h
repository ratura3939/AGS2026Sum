#pragma once
#include "EnemyDamageState.h"

class EnemyBlowAwayState : public EnemyDamageState
{
public:

	//コンストラクタ
	EnemyBlowAwayState(const VECTOR& _vec);

	//デストラクタ
	~EnemyBlowAwayState(void)override;

	//状態の開始
	void Enter(EnemyBase& _enemy)override;

	//状態の更新
	void Update(EnemyBase& _enemy)override;

	//状態の終了
	void Exit(EnemyBase& _enemy)override;

	//状態IDの取得
	const ENEMY_STATE GetStateId(void)const override { return ENEMY_STATE::BLOW_AWAY; }

private:

	//ダメージアニメーションの速度
	static constexpr float ANIM_SPEED = 1.0f;

	//吹っ飛び速度
	static constexpr float SPEED = 10.0f;

	//移動時間の最大値
	static constexpr float DAMAGE_MOVE_TIME_MAX = 0.3f;
};

