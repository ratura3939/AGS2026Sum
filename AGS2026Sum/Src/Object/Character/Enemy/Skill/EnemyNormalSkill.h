#pragma once
#include "EnemySkillBase.h"

class EnemyNormalSkill : public EnemySkillBase
{
public:

	//コンストラクタ
	EnemyNormalSkill(void);

	//デストラクタ
	~EnemyNormalSkill(void)override;

	//開始
	void Enter(EnemyBase& _owner)override;

	//更新
	void Update(EnemyBase& _owner)override;

	//終了
	void Exit(EnemyBase& _owner)override;

	//終了判定
	const bool IsEnd(void)const override;

private:

	//攻撃時間
	static constexpr float ATTACK_TIME = 3.0f;
};

