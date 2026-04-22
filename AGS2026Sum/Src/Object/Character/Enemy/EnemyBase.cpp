#include "../../../pch.h"
#include "../../../Utility/Utility.h"
#include "../../../Manager/GameSystem/CollisionManager.h"
#include "../../Common/Collider.h"
#include "../../Common/Geometry/Sphere.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const int _num)
	: myNum_(_num)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Load(void)
{
	//TODO:モデル差し込み
}

void EnemyBase::Draw(void)
{
}

void EnemyBase::Release(void)
{
}

void EnemyBase::HitCollider(std::weak_ptr<Collider> _col)
{
}

void EnemyBase::DoInit(void)
{
	//座標
	pos_ = Utility::VECTOR_ZERO;
	pos_.x += 80.0f * myNum_;

	//コライダの生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, BROUD_RADIUS, RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY);
}

void EnemyBase::DoUpdate(void)
{
	//移動処理
	Move();
}

void EnemyBase::DrawDebug(void)
{
#ifdef _DEBUG

	for (auto& col : colliders_) {
		col->DrawDebugCollider();
	}

#endif // DEBUG
}

void EnemyBase::Move(void)
{
}

void EnemyBase::Attack(void)
{
}
