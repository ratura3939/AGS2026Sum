#pragma once
#include<memory>
#include<DxLib.h>

class Door;

class AutoDoor
{
public:
	AutoDoor(const VECTOR& _position);
	~AutoDoor(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void OpenDoor(void);
	void CloseDoor(void);

	const VECTOR& GetPos(void) { return pos_; }
	const VECTOR& GetCameraGoalPosOfDoorEvent(void);

private:
	VECTOR pos_;			//ドアの中心座標
	float movingPower_;		//ドアの移動量(総量)

	std::unique_ptr<Door>leftDoor_;		//左ドア
	std::unique_ptr<Door>rightDoor_;	//右ドア
};

