#pragma once
#include "EnemyStateBase.h"

class EnemyKnockDownState : public EnemyStateBase
{
public:

	//コンストラクタ
	EnemyKnockDownState(void);
	
	//デストラクタ
	~EnemyKnockDownState(void)override;

	//状態の開始
	void Enter(EnemyBase& _enemy)override;

	//状態の更新
	void Update(EnemyBase& _enemy)override;
	
	//状態の終了
	void Exit(EnemyBase& _enemy)override;

	//状態IDの取得
	const ENEMY_STATE GetStateId(void)const override { return ENEMY_STATE::KNOCK_DOWN; }

private:

	//ダウン時間の最大値
	static constexpr float KNOCK_DOWN_TIME_MAX = 2.0f;

	//ダウン時間
	float knockDownTime_;
};

