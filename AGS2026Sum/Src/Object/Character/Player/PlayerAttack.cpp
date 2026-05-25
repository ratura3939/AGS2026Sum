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
	,data_()
	,animNames_()
	,nextType_(ATTACK_TYPE::MAX)
	,latestReserveType_(ATTACK_TYPE::MAX)
{
	debugColor_ = 0xffffff;
}

PlayerAttack::~PlayerAttack(void)
{
}

void PlayerAttack::DrawDebug(void)
{
	if (currentType_ == ATTACK_TYPE::MAX) {
		DrawFormatString(30, 150, 0xff0000, L"CURRENT = MAX");
	}
	else if (currentType_ == ATTACK_TYPE::PUNCH) {
		DrawFormatString(30, 150, 0xff0000, L"CURRENT = PUNCH");
	}
	else if (currentType_ == ATTACK_TYPE::KICK) {
		DrawFormatString(30, 150, 0xff0000, L"CURRENT = KICK");
	}

	if (nextType_ == ATTACK_TYPE::MAX) {
		DrawFormatString(30, 180, 0xff0000, L"NEXT = MAX");
	}
	else if (nextType_ == ATTACK_TYPE::DEBUG) {
		DrawFormatString(30, 180, 0xff0000, L"ERROR");
	}
	else if (nextType_ == ATTACK_TYPE::PUNCH) {
		DrawFormatString(30, 180, 0xff0000, L"NEXT = PUNCH");
	}
	else if (nextType_ == ATTACK_TYPE::KICK) {
		DrawFormatString(30, 180, 0xff0000, L"NEXT = KICK");
	}

	if (latestReserveType_ == ATTACK_TYPE::MAX) {
		DrawFormatString(30, 210, 0xff0000, L"LATEST = MAX");
	}
	else if (latestReserveType_ == ATTACK_TYPE::PUNCH) {
		DrawFormatString(30, 210, 0xff0000, L"LATEST = PUNCH");
	}
	else if (latestReserveType_ == ATTACK_TYPE::KICK) {
		DrawFormatString(30, 210, 0xff0000, L"LATEST = KICK");
	}

	DrawFormatString(30, 240, 0xff0000, L"LEVEL = %d", level_);
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

	if (currentData_.counter >= currentData_.time) {
		//攻撃終了
		currentData_ = {};
		colliders_[0]->SetUseThis(false);	//コライダの無効化
		return;
	}

	currentData_.counter++;
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

void PlayerAttack::ReserveAttack(const ATTACK_TYPE& _type)
{
	//レベルの上昇
	debugColor_ = PUNCH_COLOR;

	if (_type == ATTACK_TYPE::KICK) {
		if (!isKickCorrection_) {
			level_--;
			isKickCorrection_ = true;
		}
	}
	else {
		isKickCorrection_ = false;
	}

	level_++;	//レベルアップ
	nextType_ = _type;
	latestReserveType_ = _type;

	
	//レベル補正関連
	// ******************************************************************
	//攻撃種別が不正だった場合
	if (_type == ATTACK_TYPE::MAX) {
		level_ = 0;	//レベルリセット

		currentType_ = ATTACK_TYPE::MAX;
		//nextType_ = ATTACK_TYPE::MAX;
		nextType_ = ATTACK_TYPE::DEBUG;

		return;	//処理不可
	}
	
	//レベルが範囲外の場合（コンボを初段に）
	//0未満(キック時にのみ出現)
	if (level_ < 0) {
		level_ = 0;	//レベルリセット
		isKickCorrection_ = false;

		//キックは一段目からは発生不可のため、攻撃を行わないように設定
		currentType_ = ATTACK_TYPE::MAX;
		//nextType_ = ATTACK_TYPE::MAX;
		nextType_ = ATTACK_TYPE::DEBUG;
	}
	else if (level_ >= ATTACK_LEVEL_MAX) {
		isKickCorrection_ = false;
		level_ = 0;	//レベルリセット

		if (_type == ATTACK_TYPE::KICK) {
			currentType_ = ATTACK_TYPE::MAX;
			//nextType_ = ATTACK_TYPE::MAX;
			nextType_ = ATTACK_TYPE::DEBUG;
		}
		else {
			nextType_ = ATTACK_TYPE::PUNCH;	//パンチは初段攻撃のため、次の攻撃種別をパンチにする
		}
	}
}

void PlayerAttack::Attack(void)
{
	if (nextType_ == ATTACK_TYPE::MAX) {
		return;	//攻撃予約なし
	}

	currentData_ = data_[static_cast<int>(nextType_)][level_];
	if (nextType_ == ATTACK_TYPE::DEBUG) {
		currentType_ = ATTACK_TYPE::PUNCH;
		nextType_ = ATTACK_TYPE::DEBUG;		//次の攻撃種別リセット
	}
	else {
		currentType_ = nextType_;
		nextType_ = ATTACK_TYPE::MAX;		//次の攻撃種別リセット
	}
	
	ApplyAttackColliderSettings();		//情報適用;
	colliders_[0]->SetUseThis(true);	//コライダの有効化
	currentData_.counter = 0;
}

const PlayerAttack::AttackAnimationInfo PlayerAttack::GetCurrentAttackAnimInfo(void) const
{
	if(currentType_ == ATTACK_TYPE::MAX) {
		return AttackAnimationInfo();	//攻撃なし
	}

	AttackAnimationInfo ret;

	ret.name = animNames_[static_cast<int>(currentType_)][level_];
	ret.speed = data_[static_cast<int>(currentType_)][level_].animationSpeed;

	return	ret;
}

const bool PlayerAttack::IsAttacking(void) const
{
	return colliders_[0]->IsUseThis();
}
