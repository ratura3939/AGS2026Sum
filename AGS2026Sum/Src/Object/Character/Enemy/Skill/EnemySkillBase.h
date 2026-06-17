#pragma once

class EnemyBase;
class AttackDataBase;

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
	virtual const bool ReadyUpdate(EnemyBase& _owner) = 0;

	//準備終了
	virtual void ReadyExit(EnemyBase& _owner) = 0;

	//開始
	virtual void Enter(EnemyBase& _owner) = 0;

	//更新(終了判定も)
	virtual const bool Update(EnemyBase& _owner) = 0;

	//終了
	virtual void Exit(EnemyBase& _owner) = 0;

protected:

	//カウンタ
	float attackCnt_;
};

