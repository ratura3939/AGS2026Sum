#include "../../pch.h"
#include"../MakeCutSceneManager.h"
#include"EditFunction/AddNewData.h"
#include"EditFunction/EditData.h"
#include"EditFunction/MakeFile.h"
#include "EditDataScene.h"

EditDataScene::EditDataScene(void)
	:newDataFunc_(std::make_unique<AddNewData>())
	,editDataFunc_(std::make_unique<EditData>())
	,makeFileFunc_(std::make_unique<MakeFile>())
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
