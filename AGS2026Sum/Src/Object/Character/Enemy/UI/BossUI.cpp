#include "../../../../pch.h"
#include "../../../../Application.h"
#include "../Types/Boss/BossBase.h"
#include "BossUI.h"

BossUI::BossUI(void)
	:boss_(nullptr)
{
}

BossUI::~BossUI(void)
{
}

void BossUI::Draw(void)
{
	//表示するものがいないなら
	if (!boss_)return;

	//ボスUI
	Vector2 healthPos;
	Vector2 healthWH;
	healthPos.x = Application::SCREEN_SIZE_X / 2 + HP_LOCAL_POS_X;
	healthPos.y = Application::SCREEN_SIZE_Y / 2 + HP_LOCAL_POS_Y;
	healthWH.x = healthPos.x + HP_WIDTH;
	healthWH.y = healthPos.y + HP_HEIGHT;
	float hp = boss_->GetHp();
	float hpMax = boss_->GetHpMax();

	//HPバー
	DrawBox(healthPos.x - HP_WINDOW, healthPos.y - HP_WINDOW, healthWH.x + HP_WINDOW, healthWH.y + HP_WINDOW, 0x0, true);
	if (hp > 0.0f)
	{
		DrawBox(healthPos.x, healthPos.y, healthPos.x + HP_WIDTH * (hp / hpMax), healthPos.y + HP_HEIGHT, 0xff0000, true);
	}

	//ガード耐久値
	float guard = boss_->GetGuardDurability();

	if (guard < 0.0f)
	{

	}
	else
	{

	}
}

void BossUI::SetDrawBoss(const BossBase* _boss)
{
	//既に同じなら
	if (boss_ == _boss)return;

	//何もしない
	boss_ = _boss;
}