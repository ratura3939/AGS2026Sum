#include"../../pch.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Scene/Main/Game.h"
#include"PlayerChara.h"
#include "PlayerAttack.h"
#include "PlayerManager.h"

const std::wstring PlayerManager::ANIM_IDLE = L"Idle";
const std::wstring PlayerManager::ANIM_RUN = L"Run";
const std::wstring PlayerManager::ANIM_DAMAGE = L"Damage";
const std::wstring PlayerManager::ANIM_FIRST_PUNCH = L"FirstPunch";
const std::wstring PlayerManager::ANIM_SECOND_PUNCH = L"SecondPunch";
const std::wstring PlayerManager::ANIM_THIRD_PUNCH = L"ThirdPunch";
const std::wstring PlayerManager::ANIM_MIDDLE_KICK = L"MiddleKick";
const std::wstring PlayerManager::ANIM_HIGH_KICK = L"HighKick";
const std::wstring PlayerManager::ANIM_FINSH_KICK = L"FinishKick";

namespace {
	const VECTOR FOCUS_RELATIVE = { 0.0f,0.0f,300.0f };
}

PlayerManager::PlayerManager(Game& _gameScene)
	:scene_(_gameScene)
	,character_(std::make_shared<PlayerChara>())
	,attack_(nullptr)
	,isRefuseAttackInput_(false)
	,isForcePlayAnim_(false)
	,isSpecialAttackRedy_(false)
{
	character_->Load();		//キャラクターの読み込み
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	character_->Init();		//キャラクターの初期化

	attack_ = std::make_unique<PlayerAttack>(character_->GetPos(), character_->GetQua());	//攻撃クラスの生成
	attack_->Init();		//攻撃クラスの初期化

	//注視点の更新
	Camera& camera = SceneManager::GetInstance().GetCamera();
	focusPos_ = VAdd(character_->GetPos(), camera.GetRot().PosAxis(FOCUS_RELATIVE));
}

void PlayerManager::Update(void)
{
	UserInput();			//入力受付
	character_->Update();	//キャラクターの更新
	attack_->Update();		//攻撃クラスの更新

	//注視点の更新
	Camera& camera = SceneManager::GetInstance().GetCamera();
	focusPos_ = VAdd(character_->GetPos(), camera.GetRot().PosAxis(FOCUS_RELATIVE));

	//アニメーションが終了していたら(攻撃関連のアニメーションに限り発生するもの)
	if (character_->IsFinishAttackAnimation()) {

		//攻撃予約関係
		isRefuseAttackInput_ = false;	//攻撃入力を受け付ける状態にする

		if (character_->IsStartNextAttackAnimation()) {
			attack_->Attack();	//攻撃開始
		}
		else {
			//コンボの終了
			attack_->FinishAttack();	//攻撃終了処理
			character_->SetIsAttack(false);	//そうでないときは攻撃状態を解除する
		}
	}
}

void PlayerManager::Draw(void)
{
	character_->Draw();		//キャラクターの描画
	attack_->Draw();		//攻撃クラスの描画
	
	if (isSpecialAttackRedy_) {
		DrawString(30, 300, L"SpecialRedy!!!", 0xffffff);
	}

	attack_->DrawDebug();	//攻撃クラスのデバッグ描画
	DrawFormatString(30, 280, 0xffffff, L"AttackCansel = %d", static_cast<int>(isRefuseAttackInput_));	//現在の攻撃アニメーション登録名の先頭文字を表示(デバッグ用)
	character_->DrawNextAnimations();	//キャラクターの次のアニメーションのデバッグ描画
}

void PlayerManager::Release(void)
{
	character_->Release();	//キャラクターの解放
}

const VECTOR& PlayerManager::GetPos(void) const
{
	return character_->GetPos();
}

const VECTOR& PlayerManager::GetFocusPos(void)
{
	return focusPos_;
}

const Quaternion& PlayerManager::GetQua(void)
{
	return character_->GetQua();
}

const CharacterBase::KNOCKBACK_TYPE PlayerManager::GetCurrentKnockBackType(void) const
{
	const std::string knockBackTypeString = attack_->GetCurrentAttackKnockBackType();	//攻撃クラスから現在の攻撃の吹っ飛び方の文字列を取得

	using KNOCKBACK_TYPE = CharacterBase::KNOCKBACK_TYPE;

	//文字列と照らし合わせて、ノックバックの種類を返す
	if (knockBackTypeString == CharacterBase::KNOCKBACK_TYPE_STRING[static_cast<int>(KNOCKBACK_TYPE::STAGGER)]) {
		return KNOCKBACK_TYPE::STAGGER;
	}
	else if (knockBackTypeString == CharacterBase::KNOCKBACK_TYPE_STRING[static_cast<int>(KNOCKBACK_TYPE::PUSH_BACK)]) {
		return KNOCKBACK_TYPE::PUSH_BACK;
	}
	else if (knockBackTypeString == CharacterBase::KNOCKBACK_TYPE_STRING[static_cast<int>(KNOCKBACK_TYPE::LAUNCH)]) {
		return KNOCKBACK_TYPE::LAUNCH;
	}
	else if (knockBackTypeString == CharacterBase::KNOCKBACK_TYPE_STRING[static_cast<int>(KNOCKBACK_TYPE::FLOAT)]) {
		return KNOCKBACK_TYPE::FLOAT;
	}
	else if (knockBackTypeString == CharacterBase::KNOCKBACK_TYPE_STRING[static_cast<int>(KNOCKBACK_TYPE::SLAM)]) {
		return KNOCKBACK_TYPE::SLAM;
	}
	else if (knockBackTypeString == CharacterBase::KNOCKBACK_TYPE_STRING[static_cast<int>(KNOCKBACK_TYPE::BLOW_AWAY)]) {
		return KNOCKBACK_TYPE::BLOW_AWAY;
	}

	//例外
	return KNOCKBACK_TYPE();
}

void PlayerManager::SetAnimSpeedPercent(const float _percent)
{
	character_->SetAnimationSpeedPercent(_percent);
}

void PlayerManager::UserInput(void)
{
	InputManager& ins = InputManager::GetInstance();

#pragma region 攻撃
	bool isAttackInput = false;

	if (ins.IsTrigerrDown(InputManager::INPUT_COMMAND::ATTACK_NORMAL) && !isRefuseAttackInput_) {
		isAttackInput = Attack(PlayerAttack::ATTACK_TYPE::PUNCH);	//攻撃クラスに攻撃開始を伝え,結果を得る
	}
	else if (ins.IsTrigerrDown(InputManager::INPUT_COMMAND::ATTACK_STRONG) && !isRefuseAttackInput_) {
		isAttackInput = Attack(PlayerAttack::ATTACK_TYPE::KICK);	//攻撃クラスに攻撃開始を伝え,結果を得る
	}

	//入力があったとき
	if (isAttackInput) {
		SetAttackStateForCharacter();	//攻撃に関する情報をキャラクターに反映
	}

	//スペシャル
	if (ins.IsTrigerrDown(InputManager::INPUT_COMMAND::ATTACK_SPECIAL)) {
		isSpecialAttackRedy_ = true;
		scene_.StartSlow();
	}
	else if (ins.IsTrigerrUp(InputManager::INPUT_COMMAND::ATTACK_SPECIAL)) {
		isSpecialAttackRedy_ = false;
		scene_.EndSlow();
	}
#pragma endregion


#pragma region 移動
	auto moveVec = ins.GetMoveInput();	//LS・WASDの移動入力を取得

	//入力がある場合
	if (moveVec.x != 0.0f || moveVec.y != 0.0f) {
		character_->InputMoveVec(VECTOR(moveVec.x, moveVec.y, 0.0f));	//移動方向をキャラクターに渡す
	}
#pragma endregion

}

void PlayerManager::SetAttackStateForCharacter(void)
{
	auto attackAnimInfo = attack_->GetNextAttackAnimInfo();	//再生するアニメーション名取得

	//例外の場合
	if (attackAnimInfo.name.empty()) {
		return;
	}

	character_->SetIsAttack(true);	//攻撃中は攻撃状態にする

	auto seInfo = attack_->GetNextAttackSeInfo();	//SE情報取得

	//アニメーションの再生
	if (isForcePlayAnim_) {
		attack_->Attack();				//攻撃開始(コンボ始動のため即時発動)
		character_->ForcePlayAnim(attackAnimInfo.name, attackAnimInfo.speed, seInfo.seName, seInfo.timing);	//攻撃アニメを強制再生する
	}
	else {
		character_->PlayAnim(attackAnimInfo.name, attackAnimInfo.speed, seInfo.seName, seInfo.timing);	//攻撃アニメを再生する
	}
}

const bool PlayerManager::Attack(PlayerAttack::ATTACK_TYPE _type)
{
	bool isSuccess = false;	//処理成功フラグ
	isForcePlayAnim_ = false;

	//攻撃中の場合
	if (attack_->IsAttacking()) {
		//攻撃キャンセル可能な状態であれば
		if (character_->GetCurrentAnimationProgressRate() >= PlayerAttack::ATTACK_CANCEL_RATE) {
			isSuccess = attack_->ReserveAttack(_type);	//次段の攻撃予約

			//キャンセル可能状態中は一度だけ攻撃入力を受け付ける
			isRefuseAttackInput_ = true;	//攻撃入力を受け付けない状態にする
		}
	}
	else {
		//攻撃始動
		isSuccess = attack_->ReserveAttack(_type);	//初段の攻撃予約
		isForcePlayAnim_ = true;		//強制再生フラグON
	}

	return isSuccess;
}