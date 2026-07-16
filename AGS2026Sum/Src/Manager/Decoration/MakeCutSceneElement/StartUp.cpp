#include "../../pch.h"
#include "StartUp.h"

StartUp::StartUp(void)
	:isStart_(false)
{
}

StartUp::~StartUp(void)
{
}

void StartUp::Init(void)
{
}

void StartUp::Update(void)
{
	if (CheckHitKey(KEY_INPUT_M)) {
		//MakeCutSceneManager::GetInstance().ChangeFunction(MakeCutSceneManager::FUNCTION::SELECT);
		isStart_ = true;
	}

	if (isStart_) {
		if (CheckHitKey(KEY_INPUT_Y)) {
			MakeCutSceneManager::GetInstance().ChangeFunction(MakeCutSceneManager::FUNCTION::EDIT);
		}
		else if (CheckHitKey(KEY_INPUT_N)) {
			isStart_ = false;
		}
	}
}

void StartUp::Draw(void)
{
	DrawString(0, 0, L"StartUpScene", 0xff0000);

	if (isStart_) {
		const int posX = 300;
		const int posY = 200;

		DrawString(posX, posY, L"Do You Start Make CutScene??   Y/N", 0xff0000);
	}
}

void StartUp::Release(void)
{
}

void StartUp::Reset(void)
{
	isStart_ = false;
}
