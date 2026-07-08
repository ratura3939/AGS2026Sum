#pragma once
#include<DxLib.h>

class AutoDoor
{
public:
	AutoDoor(const VECTOR& _position);
	~AutoDoor(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void OpenDoor(void);
	void CloseDoor(void);

private:
	void MoveOpenDoor(void);	//ドアを開く
	void MoveCloseDoor(void);	//ドアを閉じる

	using DoorMoveFunc = void(AutoDoor::*)(void);
	DoorMoveFunc moveFunc_;	//ドアの移動関数

	VECTOR pos_;			//ドアの中心座標
	VECTOR leftDoorPos_;	//左ドアの座標
	VECTOR rightDoorPos_;	//右ドアの座標

	int leftDoorModelId_;	//左ドアのモデルID
	int rightDoorModelId_;	//右ドアのモデルID
	bool isMoving_;			//ドアが動いているか
	float movingPower_;		//ドアの移動量(総量)
};

