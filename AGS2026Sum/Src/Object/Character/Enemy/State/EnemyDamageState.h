#pragma once
#include "EnemyStateBase.h"

class EnemyDamageState : public EnemyStateBase
{
public:

	//コンストラクタ
	EnemyDamageState(void);

	//デストラクタ
	~EnemyDamageState(void)override;
	
	//状態の開始
	void Enter(EnemyBase& _enemy)override;
	
	//状態の更新
	void Update(EnemyBase& _enemy)override;
	
	//状態の終了
	void Exit(EnemyBase& _enemy)override;
	
	//状態IDの取得
	const ENEMY_STATE GetStateId(void)const override { return ENEMY_STATE::DAMAGE; }

private:

	//移動時間の最大値
	static constexpr float DAMAGE_MOVE_TIME_MAX = 1.5f;

	//ダメージを受けたときの移動時間
	float damageMoveTime_;
};