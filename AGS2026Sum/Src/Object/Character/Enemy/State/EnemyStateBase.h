#pragma once
#include"../Info/EnemyDefine.h"

class EnemyBase;

class EnemyStateBase
{
public:

	//コンストラクタ
	EnemyStateBase(void);

	//デストラクタ
	virtual ~EnemyStateBase(void);

	//状態の開始
	virtual void Enter(EnemyBase& _enemy) = 0;

	//状態の更新
	virtual void Update(EnemyBase& _enemy) = 0;

	//状態の終了
	virtual void Exit(EnemyBase& _enemy) = 0;

	//状態IDの取得
	virtual const ENEMY_STATE GetStateId(void)const = 0;
};