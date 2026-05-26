#include "../pch.h"
#include "EnemyBase.h"
#include "EnemyOnHit.h"

EnemyOnHit::EnemyOnHit(EnemyBase& _parent)
	:parent_(_parent)
{
	//当たり判定のタグごとの処理の設定
	onHit_[static_cast<int>(Collider::COL_TAG::PLAYER)] = &EnemyOnHit::HitPlayer;
	onHit_[static_cast<int>(Collider::COL_TAG::PLAYER_ATTACK)] = &EnemyOnHit::HitPlayerAttack;
	onHit_[static_cast<int>(Collider::COL_TAG::ENEMY)] = &EnemyOnHit::HitEnemy;
	onHit_[static_cast<int>(Collider::COL_TAG::ENEMY_ATTACK)] = &EnemyOnHit::HitEnemyAttack;
}

EnemyOnHit::~EnemyOnHit(void)
{
}

void EnemyOnHit::HitCollider(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitPlayer(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitPlayerAttack(const std::weak_ptr<Collider> _col)
{
	//吹っ飛び
	parent_.SetMovePow(VScale(Utility::GetMoveVec(parent_.GetPos(), _col.lock()->GetGeometry().GetColPos()), -1.0f));

	//ダメージ状態
	parent_.SetState(EnemyBase::STATE::DAMAGE);

	//ダメージ処理
}

void EnemyOnHit::HitEnemy(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitEnemyAttack(const std::weak_ptr<Collider> _col)
{
}
