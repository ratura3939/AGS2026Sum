#include "../../pch.h"
#include "../../Application.h"
#include "MissionKillTarget.h"

MissionKillTarget::MissionKillTarget(void)
	: progress_(0)
	,targetNum_(0)
	, targetType_(ENEMY_TYPE::MAX)
{
}

MissionKillTarget::~MissionKillTarget(void)
{
}

void MissionKillTarget::Update(void)
{
}

void MissionKillTarget::Draw(void)
{
	//インスタンス
	auto& app = Application::GetInstance();
	float screenWidth = static_cast<float>(app.GetWindowWidth());
	float screenHeight = static_cast<float>(app.GetWindowHeight());
	float posX = screenWidth * NAVI_UV_U;
	float posY = screenHeight * NAVI_UV_V;
	int strRow = 16;

	//描画
	DrawString(posX, posY, explanText_.c_str(), 0xffffff);
	DrawFormatString(posX, posY + strRow, 0xffffff, L"%d / %d", progress_, targetNum_);
}

const bool MissionKillTarget::IsCompleted(void) const
{
	//目標数倒したか
	return progress_ >= targetNum_;
}

void MissionKillTarget::AddProgress(const ENEMY_TYPE& _enemyType)
{
	if (_enemyType == targetType_)progress_++;
}