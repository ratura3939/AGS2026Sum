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
	,isKickCorrection_(false)
	,currentAttackName_()
	,nextAttackName_()
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

	//if (currentData_.counter >= currentData_.time) {
	//	//攻撃終了
	//	currentData_ = {};
	//	colliders_[0]->SetUseThis(false);	//コライダの無効化
	//	return;
	//}

	currentData_.counter++;
}

void PlayerAttack::LoadAttackData(void)
{
	Resource res = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_ATTACK_DATA_JSON);

	//登録
	data_.emplace(PUNCH_FIRST_KEY,res.GetData<AttackData>(PUNCH_FIRST_KEY));
	data_.emplace(PUNCH_SECOND_KEY,res.GetData<AttackData>(PUNCH_SECOND_KEY));
	data_.emplace(PUNCH_THIRD_KEY,res.GetData<AttackData>(PUNCH_THIRD_KEY));
	data_.emplace(KICK_FIRST_KEY,res.GetData<AttackData>(KICK_FIRST_KEY));
	data_.emplace(KICK_SECOND_KEY,res.GetData<AttackData>(KICK_SECOND_KEY));
	data_.emplace(KICK_THIRD_KEY,res.GetData<AttackData>(KICK_THIRD_KEY));

	//アニメーション登録
	animNames_.emplace(PUNCH_FIRST_KEY, PlayerManager::ANIM_FIRST_PUNCH);
	animNames_.emplace(PUNCH_SECOND_KEY, PlayerManager::ANIM_SECOND_PUNCH);
	animNames_.emplace(PUNCH_THIRD_KEY, PlayerManager::ANIM_THIRD_PUNCH);
	animNames_.emplace(KICK_FIRST_KEY, PlayerManager::ANIM_MIDDLE_KICK);
	animNames_.emplace(KICK_SECOND_KEY, PlayerManager::ANIM_HIGH_KICK);
	animNames_.emplace(KICK_THIRD_KEY, PlayerManager::ANIM_FINSH_KICK);

	//コンボ始動は弱パンチから
	currentData_ = data_[PUNCH_FIRST_KEY];
	currentAttackName_ = PUNCH_FIRST_KEY;

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

void PlayerAttack::CorrectionAttackLevel(const ATTACK_TYPE& _type)
{
	//キックの場合
	if (_type == ATTACK_TYPE::KICK) {
		//補正がまだされていなかったら
		if (!isKickCorrection_) {
			level_--;	//補正
			isKickCorrection_ = true;	//補正完了
		}
	}
	//パンチの場合
	else {
		//補正がされていたら
		if(isKickCorrection_) {
			level_++;	//キックの補正を戻す
			isKickCorrection_ = false;	//補正解除
		}
	}
}

void PlayerAttack::ResetAttackLevel(void)
{
	level_ = -1;
	isKickCorrection_ = false;
	currentType_ = ATTACK_TYPE::MAX;
	//nextType_ = ATTACK_TYPE::MAX;
	nextType_ = ATTACK_TYPE::DEBUG;
	latestReserveType_ = ATTACK_TYPE::MAX;
}

void PlayerAttack::Draw(void)
{
	//デバッグ表記
	if (colliders_[0]->IsUseThis()) {
		DrawSphere3D(pos_, currentData_.radius, 16, debugColor_, debugColor_, false);	//コライダーの描画
	}
	

	//コンボ分岐の表記
}

void PlayerAttack::Release(void)
{
}

void PlayerAttack::HitCollider(std::weak_ptr<Collider> _col)
{
}

const bool PlayerAttack::ReserveAttack(const ATTACK_TYPE& _type)
{
	////レベルの上昇

	//CorrectionAttackLevel(_type);	//レベル補正

	//level_++;	//レベルアップ
	//nextType_ = _type;
	//latestReserveType_ = _type;

	//
	////レベル補正関連
	//// ******************************************************************
	////レベル最大後、パンチの攻撃が繰り出されていた場合
	//if (level_ >= ATTACK_LEVEL_MAX && _type == ATTACK_TYPE::PUNCH) {
	//	level_ = 0;	//初段の設定
	//	nextType_ = _type;
	//	latestReserveType_ = _type;
	//	return true;	//初段設定のためここで終了
	//}

	////レベルが範囲外の場合
	//if (level_ < 0 || level_ >= ATTACK_LEVEL_MAX || _type == ATTACK_TYPE::MAX) {
	//	ResetAttackLevel();	//レベルリセット
	//	return false;
	//}


	//次の攻撃の予約
	//例外
	if (_type == ATTACK_TYPE::MAX) {
		return false;	//攻撃予約なし
	}

	//現在の攻撃が最終段である場合

	std::string nextAttackKey = data_[currentAttackName_].nextAttacks[static_cast<int>(_type)];

	//次の攻撃が設定されていない場合(コンボの最終段、または引数タイプに派生がない)
	if (nextAttackKey == "") {
		//初段はパンチのみのため、入力がパンチだった場合,それに設定
		if (_type == ATTACK_TYPE::PUNCH) {
			nextAttackKey = PUNCH_FIRST_KEY;	//初段のパンチ設定
		}
		else {
			nextAttackName_ = "";	//空を設定
		}
	}
	//次の攻撃が設定されている場合
	else {
		nextAttackName_ = nextAttackKey;	//次の攻撃アニメーション登録名の設定
	}
	
	return true;
}

void PlayerAttack::Attack(void)
{
	debugColor_ = PUNCH_COLOR;
	if (nextType_ == ATTACK_TYPE::KICK) {
		debugColor_ = KICK_COLOR;
	}

	if (nextAttackName_ == "") {
		return;	//次の攻撃が設定されていない
	}

	//currentData_ = data_[static_cast<int>(nextType_)][level_];
	currentData_ = data_[nextAttackName_];

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

const PlayerAttack::AttackAnimationInfo PlayerAttack::GetNextAttackAnimInfo(void) const
{
	if(nextType_ == ATTACK_TYPE::MAX) {
		return AttackAnimationInfo();	//攻撃なし
	}

	AttackAnimationInfo ret;

	ret.name = animNames_.at(nextAttackName_);
	ret.speed = data_.at(nextAttackName_).animationSpeed;

	return	ret;
}

const bool PlayerAttack::IsAttacking(void) const
{
	return colliders_[0]->IsUseThis();
}

void PlayerAttack::FinishAttack(void)
{
	currentData_ = {};
	colliders_[0]->SetUseThis(false);	//コライダの無効化

	//レベルリセット関連
	ResetAttackLevel();
}
