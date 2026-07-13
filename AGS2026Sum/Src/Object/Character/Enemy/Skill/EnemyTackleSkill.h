#pragma once
#include "EnemySkillBase.h"
class EnemyTackleSkill :
    public EnemySkillBase
{
public:

	//コンストラクタ
	EnemyTackleSkill(void);

	//デストラクタ
	~EnemyTackleSkill(void)override;

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
	const AttackDataBase::ATTACK_ELEMENT GetAttackElement(void)const override { return AttackDataBase::ATTACK_ELEMENT::TACKLE; };

private:

	//アニメーションスピード
	static constexpr float TACKLE_ANIM_SPEED = 5.0f;
	static constexpr float TACKLE_SPEED = 15.0f;

	//攻撃時間
	static constexpr float ATTACK_READY_TIME = 2.0f;
	static constexpr float ATTACK_TIME = 2.0f;
	static constexpr float ATTACK_END_TIME = 1.0f;

	//攻撃の相対座標
	static constexpr VECTOR ATTACK_LOCAL_POS = { 0.0f, 0.0f, 0.0f };

	//攻撃の半径
	static constexpr float RADIUS = 200.0f;
};

