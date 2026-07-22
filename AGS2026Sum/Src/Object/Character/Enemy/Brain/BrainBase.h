#pragma once
#include "../Info/EnemyDefine.h"

class EnemyBase;
class EnemySkillBase;

class BrainBase
{
public:

	//コンストラクタ
	BrainBase(EnemyBase& _parent);
	
	//デストラクタ
	virtual~BrainBase(void);

	//優先度決定
	virtual void DecidePriority(void) = 0;

	//優先度で行動選択
	void ChoiceAction(void);

	//攻撃スキル選択
	virtual void ChoiceAttackSkill(void) = 0;

protected:

	//行動優先度係数
	static constexpr int PRIORITY = 1;

	//親
	EnemyBase& parent_;

	//敵の行動優先度(値が高いほど優先する)
	std::array<int, static_cast<int>(ENEMY_ACTION::MAX)> actionPriority_;
};

