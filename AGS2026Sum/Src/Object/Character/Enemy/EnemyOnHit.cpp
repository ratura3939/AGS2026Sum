#include "../pch.h"
#include "EnemyBase.h"
#include "State/EnemyDamageState.h"
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
	//当たり判定のタグごとの処理の呼び出し
	((this)->*onHit_[static_cast<int>(_col.lock()->GetTags())])(_col);
}

void EnemyOnHit::HitPlayer(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitPlayerAttack(const std::weak_ptr<Collider> _col)
{
	//吹っ飛び
	VECTOR blowPow = VScale(Utility::GetMoveVec(parent_.GetPos(), _col.lock()->GetGeometry().GetColPos()), -BLOW_POWER);
	blowPow.y = 0.0f;
	parent_.SetMovePow(blowPow);

	//ダメージ状態
	parent_.ChangeState(std::make_unique<EnemyDamageState>());

	//ダメージ処理
	parent_.Damage(4.0f);
}

void EnemyOnHit::HitEnemy(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitEnemyAttack(const std::weak_ptr<Collider> _col)
{
}
