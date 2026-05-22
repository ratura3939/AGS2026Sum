#pragma once
#include <unordered_map>
#include"EnemyBase.h"

class EnemyBrain
{
public:

	//コンストラクタ
	EnemyBrain(EnemyBase& _parent);

	//デストラクタ
	~EnemyBrain(void);

	//優先度決定
	void DecidePriority(void);

	//優先度で行動選択
	void ChoiceAction(void);

private:

	//行動優先度係数
	static constexpr int PRIORITY = 1;

	//親
	EnemyBase& parent_;

	//敵の行動優先度(値が高いほど優先する)
	std::array<int, static_cast<int>(EnemyBase::ENEMY_ACTION::MAX)> actionPriority_;
	std::array<std::function<void(void)>, static_cast<int>(EnemyGroup::GROUP_ORDER::MAX)> orderPriority_;

	//グループの命令ごとの行動優先度の設定
	void OrderStayPriority(void);
	void OrderMovePriority(void);
	void OrderAttackReadyPriority(void);
};
