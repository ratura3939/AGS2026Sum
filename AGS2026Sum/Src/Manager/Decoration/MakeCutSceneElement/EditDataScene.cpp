#include "../../pch.h"
#include"../MakeCutSceneManager.h"
#include "EditDataScene.h"

EditDataScene::EditDataScene(void)
{
}

EditDataScene::~EditDataScene(void)
{
}

void EditDataScene::Init(void)
{
}

void EditDataScene::Update(void)
{
	if (CheckHitKey(KEY_INPUT_N)) {
		MakeCutSceneManager::GetInstance().ChangeFunction(MakeCutSceneManager::FUNCTION::START_UP);
	}
}

void EditDataScene::Draw(void)
{
	DrawString(0, 0, L"EditDataScene", 0xff0000);
}

void EditDataScene::Release(void)
{
}

void EditDataScene::Reset(void)
{
}
