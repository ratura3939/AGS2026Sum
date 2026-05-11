#include "../../../pch.h"
#include"../../Common/Geometry/Sphere.h"
#include "PlayerAttack.h"

PlayerAttack::PlayerAttack(const VECTOR& _playerPos)
	:ActorBase()
	,playerPos_(_playerPos)
	,currentData_()
	,level_(0)
	,counter_(0)
	,data_()
{
}

PlayerAttack::~PlayerAttack(void)
{
}

void PlayerAttack::DoInit(void)
{
	LoadAttackData();
	colliders_.push_back(std::make_shared<Collider>(*this, Collider::COL_TAG::PLAYER_ATTACK, std::make_unique<Sphere>(pos_, pos_, currentData_.scale, currentData_.scale), std::set<Collider::COL_TAG>{ Collider::COL_TAG::ENEMY }));
}

void PlayerAttack::DoUpdate(void)
{
	if (counter_ >= currentData_.time) {
		//攻撃終了
		currentData_ = {};
		colliders_[0]->SetUseThis(false);	//コライダの無効化
		return;
	}

	counter_++;
}

void PlayerAttack::LoadAttackData(void)
{

	//コンボ始動は弱パンチから
	currentData_ = data_[static_cast<int>(ATTACK_TYPE::PUNCH)][level_];

	ApplyAttackColliderSettings();	//情報適用
}

void PlayerAttack::ApplyAttackColliderSettings(void)
{
	power_ = currentData_.power;	//攻撃力変更

	Sphere& attackSphere = colliders_[0]->GetGeometry<Sphere>();
	
	attackSphere.SetRadius(currentData_.scale);	//半径変更
}

void PlayerAttack::Draw(void)
{
	//コンボ分岐の表記
}

void PlayerAttack::Release(void)
{
}

void PlayerAttack::HitCollider(std::weak_ptr<Collider> _col)
{
}

void PlayerAttack::Attack(const ATTACK_TYPE& _type)
{
	int useLevel = level_;
	if (_type == ATTACK_TYPE::KICK) {
		useLevel--;	//キックはレベル1から
	}

	if (useLevel < 0 || 
		useLevel > ATTACK_LEVEL_MAX||
		(useLevel >= ATTACK_LEVEL_MAX&&_type==ATTACK_TYPE::PUNCH)) {
		return;	//処理不可
	}

	currentData_ = data_[static_cast<int>(_type)][useLevel];
	ApplyAttackColliderSettings();		//情報適用);
	colliders_[0]->SetUseThis(true);	//コライダの有効化

	level_++;	//レベルアップ
}