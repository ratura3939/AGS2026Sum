#pragma once
#include <unordered_map>
#include "../Info/EnemyDefine.h"
#include "BrainBase.h"

class EnemyBase;

class MiddleBossBrain : public BrainBase
{
public:

	//コンストラクタ
	MiddleBossBrain(EnemyBase& _parent);

	//デストラクタ
	~MiddleBossBrain(void)override;

	//優先度決定
	void DecidePriority(void)override;

	//攻撃スキル選択
	void ChoiceAttackSkill(void)override;

private:

	//関数ポインタ
	using Func = void(MiddleBossBrain::*)(void);

	//優先度決め
	std::array<Func, static_cast<int>(GROUP_ORDER::MAX)> orderPriority_;

	//グループの命令ごとの行動優先度の設定
	void OrderStayPriority(void);
	void OrderMovePriority(void);
	void OrderAlertPriority(void);

};

