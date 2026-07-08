#include "../../pch.h"
#include"../../Manager/Generic/ResourceManager.h"
#include "AutoDoor.h"

AutoDoor::AutoDoor(const VECTOR& _position)
	:pos_(_position)
	,leftDoorModelId_(-1)
	,rightDoorModelId_(-1)
	,isOpen_(false)
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
}

void AutoDoor::Update(void)
{
}

void AutoDoor::Draw(void)
{
}

void AutoDoor::Release(void)
{
}

void AutoDoor::OpenDoor(void)
{
}

void AutoDoor::CloseDoor(void)
{
}
