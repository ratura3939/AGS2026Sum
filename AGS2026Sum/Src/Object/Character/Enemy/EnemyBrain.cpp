#include "../../../pch.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"
#include "EnemyBrain.h"

EnemyBrain::EnemyBrain(EnemyBase& _parent)
	:parent_(_parent)
{
	//グループの命令ごとの行動優先度の設定
	orderPriority_[static_cast<int>(GROUP_ORDER::STAY)] = &EnemyBrain::OrderStayPriority;
	orderPriority_[static_cast<int>(GROUP_ORDER::MOVE)] = &EnemyBrain::OrderMovePriority;
	orderPriority_[static_cast<int>(GROUP_ORDER::ALERT)] = &EnemyBrain::OrderAlertPriority;
}

EnemyBrain::~EnemyBrain(void)
{
}

void EnemyBrain::DecidePriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//行動優先度の初期化
	actionPriority_.fill(0);

	//グループの命令
	const EnemyGroup* group = parent_.GetGroup();
	int order = static_cast<int>(group->GetOrder());

	//グループから一定距離以上離れているならグループに戻る行動を優先する
	if (Utility::SqrMagnitude(parent_.GetPos(), group->GetPos()) > EnemyBase::LEAVE_GROUP_DIST * EnemyBase::LEAVE_GROUP_DIST)
	{
		//グループに戻る行動を優先する
		actionPriority_[static_cast<int>(ENEMY_ACTION::RETURN_GROUP)] = PRIORITY;
		return;
	}

	//グループの命令ごとの行動優先度の設定
	(this->*orderPriority_[order])();
}

void EnemyBrain::ChoiceAction(void)
{
	//行動優先度の高い行動を選択する
	auto max = std::max_element(actionPriority_.begin(), actionPriority_.end());

	//優先度が0なら何もしない
	if (*max == 0)return;
	
	//優先行動を選択する
	parent_.ChangeAction(std::distance(actionPriority_.begin(), max));
}

void EnemyBrain::OrderStayPriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//グループの命令が待機なら待機行動を優先する
	actionPriority_[static_cast<int>(ENEMY_ACTION::STAY)] = PRIORITY;
}

void EnemyBrain::OrderMovePriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//グループの命令が移動なら移動行動を優先する
	actionPriority_[static_cast<int>(ENEMY_ACTION::MOVE)] = PRIORITY;
}

void EnemyBrain::OrderAlertPriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//目標地点
	const VECTOR& goalPos = parent_.GetGroup()->GetGoalPos();

	//プレイヤー(目標地点)との距離に応じて攻撃か警戒行動を優先する
	if(Utility::SqrMagnitude(parent_.GetPos(), goalPos) < EnemyBase::ATTACK_RADIUS * EnemyBase::ATTACK_RADIUS)
	{
		//攻撃行動を優先する
		actionPriority_[static_cast<int>(ENEMY_ACTION::ATTACK_READY)] = PRIORITY;
	}
	else
	{
		//警戒行動を優先する
		actionPriority_[static_cast<int>(ENEMY_ACTION::ALERT)] = PRIORITY;
	}
}