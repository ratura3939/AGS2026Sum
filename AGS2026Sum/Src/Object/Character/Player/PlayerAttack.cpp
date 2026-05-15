#include "../../../pch.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../Common/Geometry/Sphere.h"
#include"PlayerManager.h"
#include "PlayerAttack.h"

namespace {
	//攻撃データのキー(PlayerAttackData.jsonより参照)
	const std::string PUNCH_FIRST_KEY = "Punch_First";
	const std::string PUNCH_SECOND_KEY = "Punch_Second";
	const std::string PUNCH_THIRD_KEY = "Punch_Third";
	const std::string KICK_FIRST_KEY = "Kick_First";
	const std::string KICK_SECOND_KEY = "Kick_Second";
	const std::string KICK_THIRD_KEY = "Kick_Third";

	const int KICK_COLOR = 0x0000ff;
	const int PUNCH_COLOR = 0x00ff00;
}

PlayerAttack::PlayerAttack(const VECTOR& _playerPos, const Quaternion& _playerQuaRot)
	:ActorBase()
	,playerPos_(_playerPos)
	,playerQuaRot_(_playerQuaRot)
	,currentData_()
	,currentType_(ATTACK_TYPE::MAX)
	,level_(-1)
	,counter_(0)
	,data_()
{
	debugColor_ = 0xffffff;
}

PlayerAttack::~PlayerAttack(void)
{
}

void PlayerAttack::DoLoad(void)
{
}

void PlayerAttack::DoInit(void)
{
	colliders_.push_back(std::make_shared<Collider>(*this, Collider::COL_TAG::PLAYER_ATTACK, std::make_unique<Sphere>(pos_, pos_, currentData_.radius, currentData_.radius), std::set<Collider::COL_TAG>{ Collider::COL_TAG::ENEMY }));
	LoadAttackData();

	colliders_[0]->SetUseThis(false);	//コライダの無効化
}

void PlayerAttack::DoUpdate(void)
{
	pos_ = VAdd(playerPos_, playerQuaRot_.PosAxis(currentData_.localPos));	//プレイヤーの座標にローカル座標を加算して攻撃の座標とする

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
	Resource res = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_ATTACK_DATA_JSON);

	//登録
	data_[static_cast<int>(ATTACK_TYPE::PUNCH)][0] = res.GetData<AttackData>(PUNCH_FIRST_KEY);
	data_[static_cast<int>(ATTACK_TYPE::PUNCH)][1] = res.GetData<AttackData>(PUNCH_SECOND_KEY);
	data_[static_cast<int>(ATTACK_TYPE::PUNCH)][2] = res.GetData<AttackData>(PUNCH_THIRD_KEY);
	data_[static_cast<int>(ATTACK_TYPE::KICK)][0] = res.GetData<AttackData>(KICK_FIRST_KEY);
	data_[static_cast<int>(ATTACK_TYPE::KICK)][1] = res.GetData<AttackData>(KICK_SECOND_KEY);
	data_[static_cast<int>(ATTACK_TYPE::KICK)][2] = res.GetData<AttackData>(KICK_THIRD_KEY);

	//アニメーション登録
	animNames_[static_cast<int>(ATTACK_TYPE::PUNCH)][0] = PlayerManager::ANIM_FIRST_PUNCH;
	animNames_[static_cast<int>(ATTACK_TYPE::PUNCH)][1] = PlayerManager::ANIM_SECOND_PUNCH;
	animNames_[static_cast<int>(ATTACK_TYPE::PUNCH)][2] = PlayerManager::ANIM_THIRD_PUNCH;
	animNames_[static_cast<int>(ATTACK_TYPE::KICK)][0] = PlayerManager::ANIM_MIDDLE_KICK;
	animNames_[static_cast<int>(ATTACK_TYPE::KICK)][1] = PlayerManager::ANIM_HIGH_KICK;
	animNames_[static_cast<int>(ATTACK_TYPE::KICK)][2] = PlayerManager::ANIM_FINSH_KICK;

	//コンボ始動は弱パンチから
	currentData_ = data_[static_cast<int>(ATTACK_TYPE::PUNCH)][level_];

	ApplyAttackColliderSettings();	//情報適用
}

void PlayerAttack::ApplyAttackColliderSettings(void)
{
	power_ = currentData_.power;	//攻撃力変更

	Sphere* attackSphere = colliders_[0]->GetGeometry<Sphere>();
	
	if (attackSphere != nullptr) {
		attackSphere->SetRadius(currentData_.radius);	//半径変更
	}
}

void PlayerAttack::Draw(void)
{
	//デバッグ表記
	if (colliders_[0]->IsUseThis()) {
		DrawSphere3D(pos_, currentData_.radius, 16, debugColor_, debugColor_, false);
	}
	

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
	debugColor_ = PUNCH_COLOR;

	if (_type == ATTACK_TYPE::MAX) {
		level_ = -1;	//レベルリセット
		currentType_ = ATTACK_TYPE::MAX;
		return;	//処理不可
	}

	level_++;	//レベルアップ

	int useLevel = level_;
	if (_type == ATTACK_TYPE::KICK) {
		useLevel--;	//キックは段階1から使用可能
		debugColor_ = KICK_COLOR;
	}

	if (useLevel >= ATTACK_LEVEL_MAX && _type == ATTACK_TYPE::PUNCH) {
		level_ = 0;	//レベルリセット(即時使用のため0に)
		useLevel = level_;
	}

	if (useLevel < 0 || 
		useLevel >= ATTACK_LEVEL_MAX) {
		level_ = -1;	//レベルリセット
		return;	//処理不可
	}

	currentData_ = data_[static_cast<int>(_type)][useLevel];
	currentType_ = _type;
	ApplyAttackColliderSettings();		//情報適用);
	colliders_[0]->SetUseThis(true);	//コライダの有効化
	counter_ = 0;
}

const std::wstring& PlayerAttack::GetCurrentAttackAnimName(void) const
{
	int useLevel = level_;

	if (currentType_ == ATTACK_TYPE::KICK) {
		useLevel--;
	}

	return	animNames_[static_cast<int>(currentType_)][useLevel];
}

const bool PlayerAttack::IsAttacking(void) const
{
	return colliders_[0]->IsUseThis();
}
