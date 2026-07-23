#pragma once
#include "EnemySkillBase.h"

class EnemyJumpSkill : public EnemySkillBase
{
public:

	//コンストラクタ
	EnemyJumpSkill(void);

	//デストラクタ
	~EnemyJumpSkill(void)override;

	//準備開始
	void ReadyEnter(EnemyBase& _owner)override;

	//更新
	const bool ReadyUpdate(EnemyBase& _owner)override;

	//終了
	void ReadyExit(EnemyBase& _owner)override;

	//開始
	void Enter(EnemyBase& _owner)override;

	//更新
	const bool Update(EnemyBase& _owner)override;

	//終了
	void Exit(EnemyBase& _owner)override;

	//終了開始
	void EndEnter(EnemyBase& _owner)override;

	//終了更新
	const bool EndUpdate(EnemyBase& _owner)override;

	//完全終了
	void EndExit(EnemyBase& _owner)override;

	//攻撃属性の取得
	const AttackDataBase::ATTACK_ELEMENT GetAttackElement(void)const override { return AttackDataBase::ATTACK_ELEMENT::JUMP; };

private:

	//攻撃時間
	static constexpr float ATTACK_READY_TIME = 3.0f;
	static constexpr float ATTACK_TIME = 0.5f;
	static constexpr float ATTACK_END_TIME = 1.0f;

	//攻撃の相対座標
	static constexpr VECTOR ATTACK_LOCAL_POS = { 0.0f, 0.0f, 0.0f };

	//攻撃の半径
	static constexpr float RADIUS = 1000.0f;

	//エフェクトの大きさ
	static constexpr float AURA_SCALE = 10.0f;
	static constexpr float EFF_SCALE = 100.0f;
};

