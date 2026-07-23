#include "../../../pch.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include "AttackCommandInfo.h"

namespace {
	const VECTOR START_POSITION = { 700.0f,500.0f,0.0f };	//初期位置
	const VECTOR GOAL_POSITION = { 600.0f,500.0f,0.0f };	//目標位置

	const float MOVE_SPEED = 5.0f;	//スピード


	const float DIFF_GUIDE_FOR_CENTER = 100.0f;	//ガイド項目の中心からの相対距離
	const VECTOR DIFF_GUIDE_BUTON[static_cast<int>(AttackCommandInfo::BUTTON_TYPE::MAX)] = {
		{DIFF_GUIDE_FOR_CENTER,0.0f,0.0f},	//B
		{0.0f,DIFF_GUIDE_FOR_CENTER,0.0f},	//A
		{-DIFF_GUIDE_FOR_CENTER,0.0f,0.0f},	//X
		{0.0f,-DIFF_GUIDE_FOR_CENTER,0.0f}	//Y
	};
}

AttackCommandInfo::AttackCommandInfo(void)
	:useType_(TYPE::MAX)
	,commandImages_{-1,-1}
	,commandGuides_{{-1,-1,-1,-1},{-1,-1,-1,-1}}
	,isDrawGuide_(false)
	,centerPos_(Utility::VECTOR_INIT)
	,goalPos_(Utility::VECTOR_INIT)
{
}

AttackCommandInfo::~AttackCommandInfo(void)
{
}

void AttackCommandInfo::Init(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	using SOURCE = ResourceManager::SRC;

	//４ボタンの画像
	commandImages_[static_cast<int>(TYPE::SEPECIAL)] = resM.Load(SOURCE::OPERATION_ATK_IMG).handleId_;
	commandImages_[static_cast<int>(TYPE::ULTIMATE)] = resM.Load(SOURCE::OPERATION_ULT_IMG).handleId_;

	//効果のあるボタンにのみ画像を挿入
	commandGuides_[static_cast<int>(TYPE::SEPECIAL)][static_cast<int>(BUTTON_TYPE::X)] = resM.Load(SOURCE::TAKCLE_ATK_ICON_IMG).handleId_;
	commandGuides_[static_cast<int>(TYPE::SEPECIAL)][static_cast<int>(BUTTON_TYPE::Y)] = resM.Load(SOURCE::JUMP_ATK_ICON_IMG).handleId_;
}

void AttackCommandInfo::Update(void)
{
	(this->*useUpdate_)();
}

void AttackCommandInfo::Draw(void)
{
	if (useType_ == TYPE::MAX) {
		return;
	}

	//４ボタン表示
	DrawCommand();

	if (isDrawGuide_) {
		//ガイド表示
		DrawGuide();
	}
}

void AttackCommandInfo::Appear(const TYPE& _type)
{
	useType_ = _type;

	centerPos_ = START_POSITION;
	goalPos_ = GOAL_POSITION;

	useUpdate_ = &AttackCommandInfo::UpdateAppear;
}

void AttackCommandInfo::Disappear(void)
{
	isDrawGuide_ = false;
	centerPos_ = GOAL_POSITION;
	goalPos_ = START_POSITION;

	useUpdate_ = &AttackCommandInfo::UpdateDisappear;
}

void AttackCommandInfo::UpdateNone(void)
{
}

void AttackCommandInfo::UpdateAppear(void)
{
	//出現（右から左へ）
	centerPos_.x -= MOVE_SPEED;

	//ゴールについたら
	if (centerPos_.x <= goalPos_.x) {
		centerPos_ = goalPos_;
		useUpdate_ = &AttackCommandInfo::UpdateShowGuide;
	}
}

void AttackCommandInfo::UpdateShowGuide(void)
{
}

void AttackCommandInfo::UpdateDisappear(void)
{
	//消失（左から右へ）
	centerPos_.x += MOVE_SPEED;

	//ゴールについたら
	if (centerPos_.x >= goalPos_.x) {
		centerPos_ = goalPos_;
		useUpdate_ = &AttackCommandInfo::UpdateNone;
		useType_ = TYPE::MAX;
	}
}

void AttackCommandInfo::DrawCommand(void)
{
	const double exRate = 0.5;
	const double angle = 0.0;
	DrawRotaGraph(static_cast<int>(centerPos_.x), static_cast<int>(centerPos_.y), exRate, angle, commandImages_[static_cast<int>(useType_)], true);
}

void AttackCommandInfo::DrawGuide(void)
{
	const double exRate = 0.3;
	const double angle = 0.0;

	for (int i = 0;i < static_cast<int>(BUTTON_TYPE::MAX);i++) {
		VECTOR drawPos = VAdd(centerPos_, DIFF_GUIDE_BUTON[i]);
		DrawRotaGraph(static_cast<int>(drawPos.x), static_cast<int>(drawPos.y), exRate, angle, commandGuides_[static_cast<int>(useType_)][i], true);
	}
}
