#pragma once
#include "EnemyStateBase.h"

class EnemyNormalState : public EnemyStateBase
{
public:

	//コンストラクタ
	EnemyNormalState(void);
	
	//デストラクタ
	~EnemyNormalState(void)override;

	//状態の開始
	void Enter(EnemyBase& _enemy)override;

	//状態の更新
	void Update(EnemyBase& _enemy)override;
	
	//状態の終了
	void Exit(EnemyBase& _enemy)override;

	//状態IDの取得
	const ENEMY_STATE GetStateId(void)const override { return ENEMY_STATE::NORMAL; }

private:
};