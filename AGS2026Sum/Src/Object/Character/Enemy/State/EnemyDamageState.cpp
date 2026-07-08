#include "../../../../pch.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../EnemyBase.h"
#include "EnemyDamageState.h"

EnemyDamageState::EnemyDamageState(const VECTOR& _vec)
	: moveVec_(_vec)
	, damageMoveTime_(0.0f)
{
}

EnemyDamageState::~EnemyDamageState(void)
{
}