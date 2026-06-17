#include "../pch.h"
#include "../EnemyBase.h"
#include "../State/EnemyDamageState.h"
#include "EnemyOnHit.h"

EnemyOnHit::EnemyOnHit(EnemyBase& _parent)
	: parent_(_parent)
{
}

EnemyOnHit::~EnemyOnHit(void)
{
}

void EnemyOnHit::CalcDamage(const std::weak_ptr<Collider> _col)
{
	//ヒット者
	const auto& col = _col.lock();

	//吹っ飛び(相手の向いている方向)
	VECTOR blowPow = VScale(col->GetGeometry().GetColRot().GetForward(), -BLOW_POWER);

	//ダメージ状態
	parent_.ChangeState(std::make_unique<EnemyDamageState>());

	//ダメージ処理
	parent_.Damage(4.0f);
}

void EnemyOnHit::HitPlayer(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitPlayerAttack(const std::weak_ptr<Collider> _col)
{
	//ダメージ処理
	CalcDamage(_col);
}

void EnemyOnHit::HitEnemy(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitEnemyAttack(const std::weak_ptr<Collider> _col)
{
}
