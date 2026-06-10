#pragma once

class EnemyBase;

class EnemySkillBase
{
public:

	//コンストラクタ
	EnemySkillBase(void);

	//デストラクタ
	virtual~EnemySkillBase(void);

	//準備用開始
	virtual void ReadyEnter(EnemyBase& _owner) = 0;

	//準備用更新
	virtual void ReadyUpdate(EnemyBase& _owner) = 0;

	//準備終了
	virtual void ReadyExit(EnemyBase& _owner) = 0;

	//開始
	virtual void Enter(EnemyBase& _owner) = 0;

	//更新
	virtual void Update(EnemyBase& _owner) = 0;

	//終了
	virtual void Exit(EnemyBase& _owner) = 0;

	//終了判定
	virtual const bool IsEnd(void)const = 0;

protected:

	//カウンタ
	float attackCnt_;
};

