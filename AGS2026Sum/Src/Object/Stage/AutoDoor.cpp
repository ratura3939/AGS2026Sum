#include "../../pch.h"
#include"Door.h"
#include "AutoDoor.h"

namespace {
	const float DOOR_MOVE_LIMIT = 500.0f;	//ドアの移動制限
}

AutoDoor::AutoDoor(const VECTOR& _position)
	:pos_(_position)
	,movingPower_(0.0f)
{
}

AutoDoor::~AutoDoor(void)
{
}

void AutoDoor::Init(void)
{
	//左ドア
	leftDoor_ = std::make_unique<Door>(pos_, Door::DOOR_SIDE::LEFT);
	leftDoor_->Load();
	leftDoor_->Init();

	//右ドア
	rightDoor_ = std::make_unique<Door>(pos_, Door::DOOR_SIDE::RIGHT);
	rightDoor_->Load();
	rightDoor_->Init();
}

void AutoDoor::Update(void)
{
	leftDoor_->Update();
	rightDoor_->Update();

#pragma region 移動制限
	//総移動量を加算(左右に移動量の差はないので、片方のみ取得)
	movingPower_ += leftDoor_->GetMovePowerOfFrame();

	//移動上限に達していた場合
	if (movingPower_ >= DOOR_MOVE_LIMIT) {
		//ドアの状態の取得(左右に状態の差はないので、片方のみ取得)
		Door::DOOR_STATE nowState = leftDoor_->GetState();
		Door::DOOR_STATE nextState = Door::DOOR_STATE::MAX;	
		
		//開いていた時
		if (nowState == Door::DOOR_STATE::OPENING) {
			//OPEN状態に変更
			nextState = Door::DOOR_STATE::OPEN;
		}
		//閉じていた時
		else if(nowState==Door::DOOR_STATE::CLOSING) {
			//CLOSE状態に変更
			nextState = Door::DOOR_STATE::CLOSE;
		}

		//状態の変更
		leftDoor_->ChangeState(nextState);
		rightDoor_->ChangeState(nextState);

		//総移動量の初期化
		movingPower_ = 0.0f;	
	}
#pragma endregion
}

void AutoDoor::Draw(void)
{
	leftDoor_->Draw();
	rightDoor_->Draw();
}

void AutoDoor::Release(void)
{
	leftDoor_->Release();
	rightDoor_->Release();
}

void AutoDoor::OpenDoor(void)
{
	leftDoor_->ChangeState(Door::DOOR_STATE::OPENING);
	rightDoor_->ChangeState(Door::DOOR_STATE::OPENING);
}

void AutoDoor::CloseDoor(void)
{
	leftDoor_->ChangeState(Door::DOOR_STATE::CLOSING);
	rightDoor_->ChangeState(Door::DOOR_STATE::CLOSING);
}