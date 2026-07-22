#include "BrainBase.h"
#include "../../../../pch.h"
#include "../EnemyBase.h"
#include "BrainBase.h"

BrainBase::BrainBase(EnemyBase& _parent)
	: parent_(_parent)
{
}

BrainBase::~BrainBase(void)
{
}

void BrainBase::ChoiceAction(void)
{
	//行動優先度の高い行動を選択する
	auto max = std::max_element(actionPriority_.begin(), actionPriority_.end());

	//優先度が0なら何もしない
	if (*max == 0)return;

	//優先行動を選択する
	ENEMY_ACTION action = static_cast<ENEMY_ACTION>(std::distance(actionPriority_.begin(), max));

	//攻撃準備かどうか
	if (action != ENEMY_ACTION::ATTACK_READY)
	{
		//通常状態遷移
		parent_.ChangeAction(action);
	}
	else
	{
		//攻撃スキル選択
		ChoiceAttackSkill();
	}
}