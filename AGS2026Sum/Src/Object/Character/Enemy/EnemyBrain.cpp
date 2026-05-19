#include "../../../pch.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"
#include "EnemyBrain.h"

EnemyBrain::EnemyBrain(EnemyBase& _parent)
	:parent_(_parent)
{
	//グループの命令ごとの行動優先度の設定
	orderPriority_[EnemyGroup::GROUP_ORDER::STAY] = [this](void) {OrderStayPriority(); };
	orderPriority_[EnemyGroup::GROUP_ORDER::MOVE] = [this](void) {OrderMovePriority(); };
	orderPriority_[EnemyGroup::GROUP_ORDER::ATTACK_READY] = [this](void) {OrderAttackReadyPriority(); };
}

EnemyBrain::~EnemyBrain(void)
{
}

void EnemyBrain::DecidePriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//グループの命令
	const EnemyGroup* group = parent_.GetGroup();
	EnemyGroup::GROUP_ORDER order = group->GetOrder();

	//グループから一定距離以上離れているならグループに戻る行動を優先する
	if (Utility::Distance(parent_.GetPos(), group->GetPos()) < EnemyBase::LEAVE_GROUP_DIST)
	{
		//グループに戻る行動を優先する
		actionPriority_[EnemyBase::ENEMY_ACTION::RETURN_GROUP] = PRIORITY;
		return;
	}

	//グループの命令ごとの行動優先度の設定
	orderPriority_[order]();
}

void EnemyBrain::OrderStayPriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//グループの命令が待機なら待機行動を優先する
	actionPriority_[EnemyBase::ENEMY_ACTION::STAY] = PRIORITY;
}

void EnemyBrain::OrderMovePriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//グループの命令が移動なら移動行動を優先する
	actionPriority_[EnemyBase::ENEMY_ACTION::MOVE] = PRIORITY;
}

void EnemyBrain::OrderAttackReadyPriority(void)
{
	//グループに所属していない　または　グループが死んでいるなら何もしない
	if (!parent_.IsInGroup() || !parent_.IsAlive())return;

	//グループの命令が攻撃準備なら攻撃準備行動を優先する
	actionPriority_[EnemyBase::ENEMY_ACTION::ATTACK_READY] = PRIORITY;
}
