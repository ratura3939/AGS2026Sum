#include "../../pch.h"
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "MissionGoToGoal.h"

MissionGoToGoal::MissionGoToGoal(void)
	: playerPos_(nullptr)
	, distance_(0.0f)
	,targetPos_(Utility::VECTOR_INIT)
	, arrivalDistance_(0.0f)
{
}

MissionGoToGoal::~MissionGoToGoal(void)
{
}

void MissionGoToGoal::Update(void)
{
	//プレイヤー座標が設定されていない
	if (!playerPos_)return;

	//距離
	distance_ = Utility::Distance(*playerPos_, targetPos_);
}

void MissionGoToGoal::Draw(void)
{
	//インスタンス
	auto& app = Application::GetInstance();
	float screenWidth = static_cast<float>(app.GetWindowWidth());
	float screenHeight = static_cast<float>(app.GetWindowHeight());
	float posX = screenWidth * NAVI_UV_U;
	float posY = screenHeight * NAVI_UV_V;

	//描画
	DrawString(posX, posY, explanText_.c_str(), 0xffffff);
}

const bool MissionGoToGoal::IsCompleted(void) const
{
	//指定距離より近いか
	return distance_ <= arrivalDistance_;
}
