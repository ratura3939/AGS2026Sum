#include "../../../../pch.h"
#include "../EnemyBase.h"
#include "../EnemyGroup.h"
#include "MiddleBossBrain.h"

MiddleBossBrain::MiddleBossBrain(EnemyBase& _parent)
	: BrainBase(_parent)
{
}

MiddleBossBrain::~MiddleBossBrain(void)
{
}

void MiddleBossBrain::DecidePriority(void)
{
}
