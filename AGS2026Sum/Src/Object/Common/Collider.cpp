#include"../../pch.h"
#include"ActorBase.h"
#include "Collider.h"

Collider::Collider(ActorBase& _master, const COL_TAG _tag, std::unique_ptr<Geometry> _geo, const std::set<COL_TAG> _hitTags)
	: master_(_master)
	, geometry_(std::move(_geo))
	, tag_(_tag)
	, hitTags_(_hitTags)
	, isCollision_(true)
{
}

Collider::~Collider(void)
{
}

void Collider::OnHit(std::weak_ptr<Collider> _col)
{
	//死亡済み
	if (isDead_)return;

	//この当たり判定が当たった
	isHit_ = true;

	//親に相手のコライダを渡す
	master_.HitCollider(_col);

	//当たり判定が終わった
	isHit_ = false;
}

void Collider::DeleteHitTags(const COL_TAG& _tag)
{
	//タグが存在しなかったら処理しない
	if (!hitTags_.contains(_tag))return;
	hitTags_.erase(_tag);
}

const bool Collider::IsContainsHitTags(const std::set<COL_TAG>& _tags)
{
	for (auto& tag : _tags) {
		//ないものがあったら、そこで終了
		if (!hitTags_.contains(tag))return false;
	}
	return true;
}

void Collider::DrawDebugCollider(void)
{
	geometry_->Draw();
}

void Collider::SetMasterIsDraw(const bool _isDraw)
{
	//master_.SetIsDraw(_isDraw);
}