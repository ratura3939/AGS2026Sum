#pragma once
#include "EnemyStateBase.h"

class EnemyEndState : public EnemyStateBase
{
public:

	//コンストラクタ
	EnemyEndState(void);

	//デストラクタ
	~EnemyEndState(void)override;

	//状態の開始
	void Enter(EnemyBase& _enemy)override;

	//状態の更新
	void Update(EnemyBase& _enemy)override;

	//状態の終了
	void Exit(EnemyBase& _enemy)override;

	//状態IDの取得
	const ENEMY_STATE GetStateId(void)const override;

private:

	//消えるまでの時間
	static constexpr float FADE_TIME = 0.5f;

	//消えるまでの時間
	float fadeTime_;
};