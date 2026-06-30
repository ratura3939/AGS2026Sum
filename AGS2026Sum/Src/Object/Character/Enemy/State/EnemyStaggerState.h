#pragma once
#include "EnemyDamageState.h"

class EnemyStaggerState : public EnemyDamageState
{
public:

	//コンストラクタ
	EnemyStaggerState(const VECTOR& _vec);

	//デストラクタ
	~EnemyStaggerState(void)override;

	//状態の開始
	void Enter(EnemyBase& _enemy)override;

	//状態の更新
	void Update(EnemyBase& _enemy)override;

	//状態の終了
	void Exit(EnemyBase& _enemy)override;

	//状態IDの取得
	const ENEMY_STATE GetStateId(void)const override { return ENEMY_STATE::STAGGER; }

private:

	//ダメージアニメーションの速度
	static constexpr float ANIM_SPEED = 2.0f;

	//吹っ飛び速度
	static constexpr float SPEED = 0.0f;
};

