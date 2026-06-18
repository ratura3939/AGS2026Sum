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

private:

	//攻撃時間
	static constexpr float ATTACK_READY_TIME = 3.0f;
	static constexpr float JUMP_INTERVAL = 1.0f;
	static constexpr float ATTACK_TIME = 3.0f;

	//攻撃の相対座標
	static constexpr VECTOR ATTACK_LOCAL_POS = { 0.0f, 0.0f, 0.0f };

	//攻撃の半径
	static constexpr float RADIUS = 400.0f;
};

