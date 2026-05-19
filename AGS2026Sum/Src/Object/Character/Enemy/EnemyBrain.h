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

private:

	//行動優先度係数
	static constexpr int PRIORITY = 1;

	//親
	EnemyBase& parent_;

	//敵の行動優先度(値が高いほど優先する)
	std::unordered_map<EnemyBase::ENEMY_ACTION, int> actionPriority_;
	std::unordered_map<EnemyGroup::GROUP_ORDER, std::function<void(void)>> orderPriority_;

	//グループの命令ごとの行動優先度の設定
	void OrderStayPriority(void);
	void OrderMovePriority(void);
	void OrderAttackReadyPriority(void);
};
