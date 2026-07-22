#pragma once
#include "EnemyStateBase.h"

class EnemyDeathState : public EnemyStateBase
{
public:

	//コンストラクタ
	EnemyDeathState(void);
	
	//デストラクタ
	~EnemyDeathState(void)override;
	
	//状態の開始
	void Enter(EnemyBase& _enemy)override;
	
	//状態の更新
	void Update(EnemyBase& _enemy)override;
	
	//状態の終了
	void Exit(EnemyBase& _enemy)override;
	
	//状態IDの取得
	const ENEMY_STATE GetStateId(void)const override { return ENEMY_STATE::DEATH; };

private:

	//死亡許容時間
	static constexpr float DEATH_TIME = 1.5f;

	//死亡してからの時間
	float deathTime_;
};

