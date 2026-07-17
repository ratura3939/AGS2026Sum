#include "../../../../pch.h"
#include "../../../../Application.h"
#include "../../../../Renderer/PixelMaterial.h"
#include "../../../../Renderer/PixelRenderer.h"
#include "../Types/Boss/BossBase.h"
#include "BossUI.h"

BossUI::BossUI(void)
	:boss_(nullptr)
{
}

BossUI::~BossUI(void)
{
}

void BossUI::Load(void)
{
	//シェーダーの設定
	material_ = std::make_unique<PixelMaterial>(L"CircleGauge.cso", CONST_BUFF_NUM);
	material_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	material_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });

	//レンダラー
	renderer_ = std::make_unique<PixelRenderer>();
	renderer_->SetPos(Vector2(GAUGE_POS_X, GAUGE_POS_Y));
	renderer_->SetSize(Vector2(300, 300));
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
	float GuardMax = BossBattleComponent::GUARD_DURABILITY_MAX;
	float progress = guard / GuardMax;

	if (guard > 0.0f)
	{
		//定数バッファの更新
		material_->SetConstBuf(0, {GAUGE_POS_X,GAUGE_POS_Y,progress,0.0f});
		material_->SetConstBuf(1, { GAUGE_RADIUS,0.0f,0.0f,0.0f });
		renderer_->Draw(*material_);
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