#include "../../pch.h"
#include "SelectFunction.h"

SelectFunction::SelectFunction(void)
{
}

SelectFunction::~SelectFunction(void)
{
}

void SelectFunction::Init(void)
{
}

void SelectFunction::Update(void)
{
	if (CheckHitKey(KEY_INPUT_N)) {
		MakeCutSceneManager::GetInstance().ChangeFunction(MakeCutSceneManager::FUNCTION::START_UP);
	}
}

void SelectFunction::Draw(void)
{
	DrawString(0, 0, L"SelectFunctionScene", 0xff0000);
}

void SelectFunction::Release(void)
{
}

void SelectFunction::Reset(void)
{
}
