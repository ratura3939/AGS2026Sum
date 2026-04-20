#include"../../pch.h"
#include"../../Object/Common/Collider.h"
#include"../../Utility/Utility.h"
#include"../Decoration/SoundManager.h"
#include"../Generic/ResourceManager.h"
#include"../Decoration/EffectManager.h"

#include "CollisionManager.h"

using atkM = AttackManager;

CollisionManager::CollisionManager(void)
{
	isSlow_ = false;
}

CollisionManager::~CollisionManager(void)
{
}

void CollisionManager::AddCollider(std::weak_ptr<Collider> _col)
{
	colliders_.push_back(_col);
}

void CollisionManager::DeleteCollider(void)
{
	//終了したコライダを削除する
	std::erase_if(colliders_, [](const std::weak_ptr<Collider> _col) {return _col.lock()->IsDead(); });
}

void CollisionManager::DeleteAllCollider(void)
{
	colliders_.clear();
}

void CollisionManager::UpdateColliders(void)
{
	int idx = 0;
	
	for (auto& col : colliders_) {
		//使わないものは判定しない
		if (!col.lock()->IsUseThis()) {
			idx++;
			continue;
		}

		//現在以降の物としか判定をとらない(既に判定を行っているため)
		for(int i = idx + 1; i < static_cast<int>(colliders_.size()); i++) {
			//使わないものは判定しない
			if (!colliders_[i].lock()->IsUseThis())continue;
			CollisionGeometry(col, colliders_[i]);
		}
		idx++;
	}

	//削除予定リストに入っているものを削除
	DeleteCollider();
}

void CollisionManager::UseAllert(const AttackManager::ATTACK_NAME& _atkName)
{
	atkMng_.lock()->UseAllertCollision(_atkName);
}

void CollisionManager::UseAttack(const AttackManager::ATTACK_NAME& _atkName)
{
	atkMng_.lock()->UseAttackCollision(_atkName);
}

void CollisionManager::CollisionGeometry(std::weak_ptr<Collider> _col1, std::weak_ptr<Collider> _col2)
{
	//タイプの確認(双方)
	if (!CheckCollisionTags(_col1, _col2))return;
	if (!CheckCollisionTags(_col2, _col1))return;

	//前判定
	if (!PreCollision(_col1, _col2))return;
	
	//形状同士の当たり判定
	auto& geo1 = _col1.lock()->GetGeometry();
	auto& geo2 = _col2.lock()->GetGeometry();

	//衝突があった場合
	if (geo1.IsHit(geo2)) {
		_col1.lock()->OnHit(_col2);
		_col2.lock()->OnHit(_col1);
	}
}

const bool CollisionManager::CheckCollisionTags(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)
{
	//タグの確認
	auto& type2 = _col2.lock()->GetTags();

	//タグが一致するものがあったら、当たり判定を行う
	for(auto& type : _col1.lock()->GetHitTags()) {
		if (type == type2) {
			return true;
		}
	}

	return false;
}

const bool CollisionManager::PreCollision(std::weak_ptr<Collider> _col1, std::weak_ptr<Collider> _col2)
{
	//形状
	const auto& geo1 = _col1.lock()->GetGeometry();
	const auto& geo2 = _col2.lock()->GetGeometry();

	//原点
	const VECTOR& origin = geo1.GetColPos();

	//座標
	const VECTOR& pos1 = Utility::VECTOR_ZERO;
	const VECTOR& pos2 = VSub(geo2.GetColPos(), origin);

	//距離
	float range = geo1.GetBroudRadius() + geo2.GetBroudRadius();
	float sqrDistance = Utility::SqrMagnitudeF(VAdd(pos1, pos2));

	//判定
	return sqrDistance < range * range;
}
