#pragma once
#include "EnemyStateBase.h"

class EnemyDamageState : public EnemyStateBase
{
public:

	//コンストラクタ
	EnemyDamageState(const VECTOR& _vec);

	//デストラクタ
	virtual ~EnemyDamageState(void)override;
	
	//状態の開始
	virtual void Enter(EnemyBase& _enemy)override = 0;
	
	//状態の更新
	virtual void Update(EnemyBase& _enemy)override = 0;
	
	//状態の終了
	virtual void Exit(EnemyBase& _enemy)override = 0;
	
	//状態IDの取得
	virtual const ENEMY_STATE GetStateId(void)const override = 0;

protected:

	//移動時間の最大値
	static constexpr float DAMAGE_MOVE_TIME_MAX = 1.5f;

	//移動方向
	VECTOR moveVec_;

	//ダメージを受けたときの移動時間
	float damageMoveTime_;
};