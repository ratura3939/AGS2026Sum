#include "../../pch.h"
#include"../../Manager/Generic/ResourceManager.h"
#include "AutoDoor.h"

namespace {
	const VECTOR INIT_LEFT_DOOR_DIFF = { -50.0f,0.0f,0.0f };	//左ドアの初期位置の差分
	const VECTOR INIT_RIGHT_DOOR_DIFF = { 50.0f,0.0f,0.0f };	//右ドアの初期位置の差分
	const float DOOR_MOVE_SPEED = 2.0f;		//ドアの移動速度
	const float DOOR_MOVE_LIMIT = 50.0f;	//ドアの移動制限
}

AutoDoor::AutoDoor(const VECTOR& _position)
	:moveFunc_(nullptr)
	,pos_(_position)
	,leftDoorPos_(VAdd(_position, INIT_LEFT_DOOR_DIFF))
	,rightDoorPos_(VAdd(_position, INIT_RIGHT_DOOR_DIFF))
	,leftDoorModelId_(-1)
	,rightDoorModelId_(-1)
	,isMoving_(false)
	,movingPower_(0.0f)
{
}

AutoDoor::~AutoDoor(void)
{
}

void AutoDoor::Init(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	leftDoorModelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::DOOR_LEFT_MDL);
	rightDoorModelId_ = resM.LoadModelDuplicate(ResourceManager::SRC::DOOR_RIGHT_MDL);

	MV1SetPosition(leftDoorModelId_, leftDoorPos_);
	MV1SetPosition(rightDoorModelId_, rightDoorPos_);
}

void AutoDoor::Update(void)
{
	if (isMoving_) {
		(this->*moveFunc_)();	//ドアの移動関数を呼び出す

		//ドアのモデルの座標を更新
		MV1SetPosition(leftDoorModelId_, leftDoorPos_);
		MV1SetPosition(rightDoorModelId_, rightDoorPos_);

		movingPower_ += DOOR_MOVE_SPEED;	//総移動量の更新

		//移動量が制限を超えたら移動終了
		if (movingPower_ >= DOOR_MOVE_LIMIT) {
			isMoving_ = false;	//移動終了
			movingPower_ = 0.0f;	//移動量をリセット
		}
	}
}

void AutoDoor::Draw(void)
{
	MV1DrawModel(leftDoorModelId_);
	MV1DrawModel(rightDoorModelId_);
}

void AutoDoor::Release(void)
{
}

void AutoDoor::OpenDoor(void)
{
	moveFunc_ = &AutoDoor::MoveOpenDoor;
	isMoving_ = true;
}

void AutoDoor::CloseDoor(void)
{
	moveFunc_ = &AutoDoor::MoveCloseDoor;
	isMoving_ = true;
}

void AutoDoor::MoveOpenDoor(void)
{
	leftDoorPos_.x -= DOOR_MOVE_SPEED;
	rightDoorPos_.x += DOOR_MOVE_SPEED;
}

void AutoDoor::MoveCloseDoor(void)
{
	leftDoorPos_.x += DOOR_MOVE_SPEED;
	rightDoorPos_.x -= DOOR_MOVE_SPEED;
}
