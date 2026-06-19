#pragma once
#include "EnemySkillBase.h"

class EnemyNormalSkill : public EnemySkillBase
{
public:

	//コンストラクタ
	EnemyNormalSkill(void);

	//デストラクタ
	~EnemyNormalSkill(void)override;

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
	static constexpr float ATTACK_READY_TIME = 1.0f;
	static constexpr float ATTACK_TIME = 2.0f;

	//攻撃の相対座標
	static constexpr VECTOR ATTACK_LOCAL_POS = { 0.0f, 0.0f, -30.0f };

	//攻撃の半径
	static constexpr float RADIUS = 100.0f;
};

