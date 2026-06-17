#include "../../../../pch.h"
#include "../Info/EnemyDefine.h"
#include "../State/EnemyNormalState.h"
#include "../Brain/MiddleBossBrain.h"
#include "MiddleBoss.h"

MiddleBoss::MiddleBoss(void)
	: EnemyBase(ENEMY_TYPE::MIDDLE_BOSS)
{
}

MiddleBoss::~MiddleBoss(void)
{
}

void MiddleBoss::DoLoad(void)
{
	//状態の初期化
	state_ = std::make_unique<EnemyNormalState>();
	state_->Enter(*this);

	//思考の初期化
	brain_ = std::make_unique<MiddleBossBrain>(*this);

	//接触処理の初期化
	onHit_ = std::make_unique<EnemyOnHit>(*this);
}
