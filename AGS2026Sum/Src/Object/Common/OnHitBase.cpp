#include "../../pch.h"
#include "OnHitBase.h"

OnHitBase::OnHitBase(void)
{
	//当たり判定のタグごとの処理の設定
	onHit_[static_cast<int>(Collider::COL_TAG::PLAYER)] = &OnHitBase::HitPlayer;
	onHit_[static_cast<int>(Collider::COL_TAG::PLAYER_ATTACK)] = &OnHitBase::HitPlayerAttack;
	onHit_[static_cast<int>(Collider::COL_TAG::ENEMY)] = &OnHitBase::HitEnemy;
	onHit_[static_cast<int>(Collider::COL_TAG::ENEMY_ATTACK)] = &OnHitBase::HitEnemyAttack;
	onHit_[static_cast<int>(Collider::COL_TAG::STAGE)] = &OnHitBase::HitStage;
}

OnHitBase::~OnHitBase(void)
{
}

void OnHitBase::HitCollider(const std::weak_ptr<Collider> _col)
{
	//当たり判定のタグごとの処理の呼び出し
	((this)->*onHit_[static_cast<int>(_col.lock()->GetTag())])(_col);
}
