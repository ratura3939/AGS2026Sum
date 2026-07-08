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
	VECTOR pos_;
	int leftDoorModelId_;
	int rightDoorModelId_;
	bool isOpen_;
};

