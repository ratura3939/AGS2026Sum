#include"../../pch.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include"../../../Manager/GameSystem/AttackManager.h"
#include"../../../Scene/Main/Game.h"
#include"PlayerChara.h"
#include "PlayerManager.h"

const std::wstring PlayerManager::ANIM_IDLE = L"Idle";
const std::wstring PlayerManager::ANIM_RUN = L"Run";
const std::wstring PlayerManager::ANIM_FIRST_PUNCH = L"FirstPunch";
const std::wstring PlayerManager::ANIM_SECOND_PUNCH = L"SecondPunch";
const std::wstring PlayerManager::ANIM_THIRD_PUNCH = L"ThirdPunch";
const std::wstring PlayerManager::ANIM_MIDDLE_KICK = L"MiddleKick";
const std::wstring PlayerManager::ANIM_HIGH_KICK = L"HighKick";
const std::wstring PlayerManager::ANIM_FINSH_KICK = L"FinishKick";

PlayerManager::PlayerManager(Game& _gameScene)
	:scene_(_gameScene)
	,character_(std::make_shared<PlayerChara>())
{
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
	character_->Init();		//キャラクターの初期化
	//攻撃の登録
}

void PlayerManager::Update(AttackManager& _atk)
{
	UserInput();			//入力受付
	character_->Update();	//キャラクターの更新
}

void PlayerManager::Draw(void)
{
	character_->Draw();		//キャラクターの描画
}

void PlayerManager::Release(void)
{
	character_->Release();	//キャラクターの解放
}

const VECTOR& PlayerManager::GetPos(void) const
{
	return character_->GetPos();
}

const Quaternion& PlayerManager::GetQua(void)
{
	return character_->GetQua();
}

void PlayerManager::UserInput(void)
{
	InputManager& ins = InputManager::GetInstance();

#pragma region 移動
	auto moveVec = ins.GetMoveInput();	//LS・WASDの移動入力を取得

	//入力がある場合
	if (moveVec.x != 0.0f || moveVec.y != 0.0f) {
		character_->InputMoveVec(VECTOR(moveVec.x, moveVec.y, 0.0f));	//移動方向をキャラクターに渡す
	}
#pragma endregion

}
