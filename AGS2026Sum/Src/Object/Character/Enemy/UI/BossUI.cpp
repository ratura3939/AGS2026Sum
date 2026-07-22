#include "../../../../pch.h"
#include "../../../../Application.h"
#include "../../../../Manager/Generic/ResourceManager.h"
#include "../../../../Renderer/PixelMaterial.h"
#include "../../../../Renderer/PixelRenderer.h"
#include "../Types/Boss/BossBase.h"
#include "BossUI.h"

BossUI::BossUI(void)
	:boss_(nullptr)
	,isDraw_(false)
{
}

BossUI::~BossUI(void)
{
}

void BossUI::Load(void)
{
	//シェーダーの設定
	material_ = std::make_unique<PixelMaterial>(L"BossGuardGauge.cso", CONST_BUFF_NUM);
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::CRACK).handleId_);
	material_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	material_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });

	//レンダラー
	renderer_ = std::make_unique<PixelRenderer>();
	renderer_->SetPos(Vector2(0.0f,0.0f));
	renderer_->SetSize(Vector2(300, 300));
	renderer_->MakeSquereVertex();
}

void BossUI::Draw(void)
{
	//表示するものがいないなら
	if (!boss_ || !isDraw_)return;

	//ボスUI
	Vector2 healthPos;
	Vector2 healthWH;
	healthPos.x = Application::SCREEN_SIZE_X * HP_INFO.anchorX + HP_INFO.offsetX;
	healthPos.x -= HP_WIDTH / 2;
	healthPos.y = Application::SCREEN_SIZE_Y * HP_INFO.anchorY + HP_INFO.offsetY;
	healthPos.y -= HP_HEIGHT / 2;
	healthWH.x = healthPos.x + HP_WIDTH;
	healthWH.y = healthPos.y + HP_HEIGHT;
	float hp = boss_->GetHp();
	float hpMax = boss_->GetHpMax();

	//HPバー
	DrawBox(healthPos.x - HP_WINDOW, healthPos.y - HP_WINDOW, healthWH.x + HP_WINDOW, healthWH.y + HP_WINDOW, 0x0, true);
	if (hp > 0.0f)
	{
		DrawBox(healthPos.x, healthPos.y, healthPos.x + HP_WIDTH * (hp / hpMax), healthPos.y + HP_HEIGHT, 0xff8888, true);
	}

	//ガード耐久値
	float guard = boss_->GetGuardDurability();
	float GuardMax = BossBattleComponent::GUARD_DURABILITY_MAX;
	float progress = guard / GuardMax;

	//ガード耐久値が残っているなら
	if (guard > 0.0f)
	{
		//定数バッファの更新と描画
		material_->SetConstBuf(0, {GAUGE_UV_U,GAUGE_UV_V,progress,0.0f});
		material_->SetConstBuf(1, { GAUGE_RADIUS,0.0f,0.0f,0.0f });
		renderer_->Draw(*material_);
	}
	else
	{

	}
}

void BossUI::SetDrawBoss(const EnemyBase* _boss)
{
	//いないならボス表示を無効化
	if (!_boss || !_boss->IsBoss())
	{
		isDraw_ = false;
		boss_ = nullptr;
		return;
	}

	//既に同じなら表示判定のみ変更
	if (boss_ == _boss)
	{
		isDraw_ = true;
		return;
	}

	//表示ボスを変える
	boss_ = dynamic_cast<const BossBase*>(_boss);
	isDraw_ = true;
}