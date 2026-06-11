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
	const std::string SPECIAL_PUNCH_KEY = "Punch_Special";
	const std::string SPECIAL_KICK_KEY = "Kick_Special";

	const int DEFAULT_COLOR = 0xffffff;
	const int KICK_COLOR = 0x0000ff;
	const int PUNCH_COLOR = 0x00ff00;

	const int COMBO_RESET_FRAME = 30;	//コンボリセットまでのフレーム数
	const int CIRCLE_SCALE = 10;	//表記する円の大きさ

	const float COMBO_ELEMENT_DRAW_DIF_X = 50;	//コンボルートの要素の表記差分X座標
	const float COMBO_ELEMENT_DRAW_DIF_Y = 50;	//コンボルートの要素の表記差分Y座標
}

PlayerAttack::PlayerAttack(const VECTOR& _playerPos, const Quaternion& _playerQuaRot)
	:ActorBase()
	,playerPos_(_playerPos)
	,playerQuaRot_(_playerQuaRot)
	,currentData_()
	,data_()
	,animNames_()
	,latestReserveType_(ATTACK_TYPE::MAX)
	,currentAttackName_()
	,nextAttackName_()
	,comboReset_(true)
	,comboResetCounter_(0)
{
	debugColor_ = 0xffffff;
}

PlayerAttack::~PlayerAttack(void)
{
}

void PlayerAttack::DrawDebug(void)
{
}

void PlayerAttack::DoLoad(void)
{
}

void PlayerAttack::DoInit(void)
{
	MakeCollider(std::make_unique<Sphere>(pos_, pos_, currentData_.radius, currentData_.radius), Collider::COL_TAG::PLAYER_ATTACK, { Collider::COL_TAG::ENEMY });	//攻撃用のコライダ生成
	LoadAttackData();
	LoadAttackSound();

	colliders_[0]->SetUseThis(false);	//コライダの無効化
}

void PlayerAttack::DoUpdate(void)
{
	if (IsAttacking()) {
		//攻撃中は座標を更新
		pos_ = VAdd(playerPos_, playerQuaRot_.PosAxis(currentData_.localPos));	//プレイヤーの座標にローカル座標を加算して攻撃の座標とする
	}
	//攻撃していないとき
	else {
		comboResetCounter_++;	//コンボリセットカウンタを加算
		if (!comboReset_ && comboResetCounter_ > COMBO_RESET_FRAME) {
			//コンボリセットフラグが立っていない、かつコンボリセットカウンタが規定値を超えたとき
			ResetCombo();	//コンボリセット
		}
	}
	
}

void PlayerAttack::DrawComboRoute(void)
{
	const int DrawStartX = 30;	//表記開始X座標
	const int DrawStartY = 200;	//表記開始Y座標
	

	const int KickCircleDifferenceY = 50;

	int differenceDrawPosX = 0;	//表記差分X座標
	int differenceDrawPosY = 0;	//表記差分Y座標

	DrawComboRouteElement(startAttackAnimName_, { DrawStartX,DrawStartY,0 });	//コンボ始動の表記

	for (auto& info : comboRouteInfos_) {
		info.second.isDrawed = false;	//描画フラグリセット
	}
}

void PlayerAttack::DrawComboRouteElement(const std::string& _attackKey, const VECTOR& _pos)
{
	//すでに描画している場合は描画しない
	if (comboRouteInfos_[_attackKey].isDrawed) {
		return;		//終了
	}
	
	int useColor = DEFAULT_COLOR;	//デフォルト白
	if (comboRouteInfos_[_attackKey].isUsed) {
		if (comboRouteInfos_[_attackKey].type == ATTACK_TYPE::KICK) {
			useColor = KICK_COLOR;	//キックなら青
		}
		else if (comboRouteInfos_[_attackKey].type == ATTACK_TYPE::PUNCH) {
			useColor = PUNCH_COLOR;	//使用済みなら緑
		}
	}
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), CIRCLE_SCALE, useColor);
	comboRouteInfos_[_attackKey].isDrawed = true;

	std::string nextPunchAttackName = data_[_attackKey].nextAttacks[static_cast<int>(ATTACK_TYPE::PUNCH)];	//次のパンチ攻撃
	std::string nextKickAttackName = data_[_attackKey].nextAttacks[static_cast<int>(ATTACK_TYPE::KICK)];	//次のキック攻撃

	//次の要素の描画位置調整
	VECTOR nextPos = _pos;
	nextPos.x += COMBO_ELEMENT_DRAW_DIF_X;	//次の要素の表記位置をずらす

	//要素がある場合
	if (nextPunchAttackName != "") {
		//パンチ派生時、現在の攻撃がキックの場合
		if (comboRouteInfos_[_attackKey].type == ATTACK_TYPE::KICK) {
			nextPos.y -= COMBO_ELEMENT_DRAW_DIF_Y;	//パンチの要素の表記位置をさらにずらす
		}

		DrawComboRouteElement(nextPunchAttackName, nextPos);	//パンチ派生の描画
	}

	if (nextKickAttackName != "") {
		//キック派生時、現在の攻撃がパンチの場合
		if (comboRouteInfos_[_attackKey].type == ATTACK_TYPE::PUNCH) {
			nextPos.y += COMBO_ELEMENT_DRAW_DIF_Y;	//キックの要素の表記位置をさらにずらす
		}

		DrawComboRouteElement(nextKickAttackName, nextPos);	//キック派生の描画
	}
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
	data_.emplace(SPECIAL_PUNCH_KEY,res.GetData<AttackData>(SPECIAL_PUNCH_KEY));
	data_.emplace(SPECIAL_KICK_KEY,res.GetData<AttackData>(SPECIAL_KICK_KEY));

	//アニメーション登録
	animNames_.emplace(PUNCH_FIRST_KEY, PlayerManager::ANIM_FIRST_PUNCH);
	animNames_.emplace(PUNCH_SECOND_KEY, PlayerManager::ANIM_SECOND_PUNCH);
	animNames_.emplace(PUNCH_THIRD_KEY, PlayerManager::ANIM_THIRD_PUNCH);
	animNames_.emplace(KICK_FIRST_KEY, PlayerManager::ANIM_MIDDLE_KICK);
	animNames_.emplace(KICK_SECOND_KEY, PlayerManager::ANIM_HIGH_KICK);
	animNames_.emplace(KICK_THIRD_KEY, PlayerManager::ANIM_FINSH_KICK);

	//フラグ管理(コンボ履歴用)
	comboRouteInfos_.emplace(PUNCH_FIRST_KEY, ComboRouteInfo{ ATTACK_TYPE::PUNCH,false,false });
	comboRouteInfos_.emplace(PUNCH_SECOND_KEY, ComboRouteInfo{ ATTACK_TYPE::PUNCH,false,false });
	comboRouteInfos_.emplace(PUNCH_THIRD_KEY, ComboRouteInfo{ ATTACK_TYPE::PUNCH,false,false });
	comboRouteInfos_.emplace(KICK_FIRST_KEY, ComboRouteInfo{ ATTACK_TYPE::KICK,false,false });
	comboRouteInfos_.emplace(KICK_SECOND_KEY, ComboRouteInfo{ ATTACK_TYPE::KICK,false,false });
	comboRouteInfos_.emplace(KICK_THIRD_KEY, ComboRouteInfo{ ATTACK_TYPE::KICK,false,false });
	comboRouteInfos_.emplace(SPECIAL_PUNCH_KEY, ComboRouteInfo{ ATTACK_TYPE::SPECIAL,false,false });
	comboRouteInfos_.emplace(SPECIAL_KICK_KEY, ComboRouteInfo{ ATTACK_TYPE::SPECIAL,false,false });

	//コンボ始動は弱パンチから
	currentData_ = data_[PUNCH_FIRST_KEY];
	startAttackAnimName_ = PUNCH_FIRST_KEY;

	currentAttackName_ = KICK_THIRD_KEY;	//予約→発生というロジックの関係上、初段に設定するためには「次の攻撃」が設定されていない最終段を用いる

	ApplyAttackColliderSettings();	//情報適用
}

void PlayerAttack::LoadAttackSound(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	ResourceManager& resM = ResourceManager::GetInstance();
	using SOUND_TYPE = SoundManager::TYPE;
	using SOUND_NAME = SoundManager::SOUND_NAME;
	using SOURCE = ResourceManager::SRC;

	//SE読み込み
	sndM.Add(SOUND_TYPE::SE, SOUND_NAME::PUNCH_FIRST_PLAYER_SE,resM.Load(SOURCE::PUNCH_FIRST_PLAYER_SE).handleId_);
	sndM.Add(SOUND_TYPE::SE, SOUND_NAME::PUNCH_SECOND_PLAYER_SE,resM.Load(SOURCE::PUNCH_SECOND_PLAYER_SE).handleId_);
	sndM.Add(SOUND_TYPE::SE, SOUND_NAME::PUNCH_THIRD_PLAYER_SE,resM.Load(SOURCE::PUNCH_THIRD_PLAYER_SE).handleId_);
	sndM.Add(SOUND_TYPE::SE, SOUND_NAME::KICK_FIRST_PLAYER_SE,resM.Load(SOURCE::KICK_FIRST_PLAYER_SE).handleId_);
	sndM.Add(SOUND_TYPE::SE, SOUND_NAME::KICK_SECOND_PLAYER_SE,resM.Load(SOURCE::KICK_SECOND_PLAYER_SE).handleId_);
	sndM.Add(SOUND_TYPE::SE, SOUND_NAME::KICK_THIRD_PLAYER_SE,resM.Load(SOURCE::KICK_THIRD_PLAYER_SE).handleId_);

	//SE連携
	seNames_.emplace(PUNCH_FIRST_KEY, SOUND_NAME::PUNCH_FIRST_PLAYER_SE);
	seNames_.emplace(PUNCH_SECOND_KEY, SOUND_NAME::PUNCH_SECOND_PLAYER_SE);
	seNames_.emplace(PUNCH_THIRD_KEY, SOUND_NAME::PUNCH_THIRD_PLAYER_SE);
	seNames_.emplace(KICK_FIRST_KEY, SOUND_NAME::KICK_FIRST_PLAYER_SE);
	seNames_.emplace(KICK_SECOND_KEY, SOUND_NAME::KICK_SECOND_PLAYER_SE);
	seNames_.emplace(KICK_THIRD_KEY, SOUND_NAME::KICK_THIRD_PLAYER_SE);
}

void PlayerAttack::ApplyAttackColliderSettings(void)
{
	power_ = currentData_.power;	//攻撃力変更

	Sphere* attackSphere = colliders_[0]->GetGeometry<Sphere>();
	
	if (attackSphere != nullptr) {
		attackSphere->SetRadius(currentData_.radius);		//半径変更
		attackSphere->SetBroudRadius(currentData_.radius);	//前判定用半径変更
	}
}

void PlayerAttack::ResetCombo(void)
{
	currentAttackName_ = KICK_THIRD_KEY;	//現在の攻撃アニメーション登録名をリセット
	nextAttackName_ = "";	//次の攻撃アニメーション登録名をリセット
	comboReset_ = true;		//コンボリセットフラグを立てる

	ResetComboRoute();	//コンボルートのリセット
}

void PlayerAttack::ResetComboRoute(void)
{
	for (auto& info : comboRouteInfos_) {
		info.second.isUsed = false;	//使用フラグリセット
	}
}

void PlayerAttack::Draw(void)
{
	//デバッグ表記
	if (colliders_[0]->IsUseThis()) {
		DrawSphere3D(pos_, currentData_.radius, 16, debugColor_, debugColor_, false);	//コライダーの描画
	}
	

	//コンボ分岐の表記
	DrawComboRoute();
}

void PlayerAttack::Release(void)
{
}

void PlayerAttack::HitCollider(std::weak_ptr<Collider> _col)
{
}

const bool PlayerAttack::ReserveAttack(const ATTACK_TYPE& _type)
{
	//次の攻撃の予約
	//例外
	if (_type == ATTACK_TYPE::MAX) {
		return false;	//攻撃予約なし
	}

	std::string nextAttackKey = data_[currentAttackName_].nextAttacks[static_cast<int>(_type)];

	//次の攻撃が設定されていない場合(コンボの最終段、または引数タイプに派生がない)
	if (nextAttackKey == "") {
		//初段はパンチのみのため、入力がパンチだった場合,それに設定
		if (_type == ATTACK_TYPE::PUNCH) {
			nextAttackName_ = PUNCH_FIRST_KEY;	//初段のパンチ設定
		}
		//キックの場合
		else {
			nextAttackName_ = "";	//空を設定(初段にキックを許さない)
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
	if (nextAttackName_ == "") {
		return;	//次の攻撃が設定されていない
	}

	if (nextAttackName_ == startAttackAnimName_) {
		ResetComboRoute();	//コンボルートリセット
	}

	currentData_ = data_[nextAttackName_];
	
	currentAttackName_ = nextAttackName_;	//現在の攻撃アニメーション登録名の設定
	ApplyAttackColliderSettings();		//情報適用;
	colliders_[0]->SetUseThis(true);	//コライダの有効化
	comboRouteInfos_[currentAttackName_].isUsed = true;	//コンボルートの使用フラグを立てる
}

const PlayerAttack::AttackAnimationInfo PlayerAttack::GetNextAttackAnimInfo(void) const
{
	if(nextAttackName_ == "") {
		return AttackAnimationInfo();	//攻撃なし
	}

	AttackAnimationInfo ret;

	ret.name = animNames_.at(nextAttackName_);
	ret.speed = data_.at(nextAttackName_).animationSpeed;

	return	ret;
}

const PlayerAttack::AttackSeInfo PlayerAttack::GetNextAttackSeInfo(void) const
{
	if (nextAttackName_ == "") {
		return AttackSeInfo();	//攻撃なし
	}
	AttackSeInfo ret;

	ret.seName = seNames_.at(nextAttackName_);
	ret.timing = data_.at(nextAttackName_).seTiming;

	return ret;
}

const std::string& PlayerAttack::GetCurrentAttackKnockBackType(void) const
{
	return currentData_.KnockBackType;
}

const bool PlayerAttack::IsAttacking(void) const
{
	return colliders_[0]->IsUseThis();
}

void PlayerAttack::FinishAttack(void)
{
	currentData_ = {};
	colliders_[0]->SetUseThis(false);	//コライダの無効化
	comboReset_ = false;
	comboResetCounter_ = 0;
}
