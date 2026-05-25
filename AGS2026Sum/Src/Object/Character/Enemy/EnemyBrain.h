#pragma once
#include <unordered_map>
#include "EnemyDefine.h"

class EnemyBase;

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

	//関数ポインタ
	using Func = void(EnemyBrain::*)(void);

	//敵の行動優先度(値が高いほど優先する)
	std::array<int, static_cast<int>(ENEMY_ACTION::MAX)> actionPriority_;
	std::array<Func, static_cast<int>(GROUP_ORDER::MAX)> orderPriority_;

	//グループの命令ごとの行動優先度の設定
	void OrderStayPriority(void);
	void OrderMovePriority(void);
	void OrderAttackReadyPriority(void);
};
