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
	auto& app = Application::GetInstance();
	float screenWidth = static_cast<float>(app.GetWindowWidth());
	float screenHeight = static_cast<float>(app.GetWindowHeight());
	float widthRate = screenWidth / static_cast<float>(BASE_WIDTH);
	float heightRate = screenHeight / static_cast<float>(BASE_HEIGHT);
	Vector2 gaugePos;
	gaugePos.x = screenWidth * GAUGE_INFO.anchorX + GAUGE_INFO.offsetX * widthRate - GAUGE_SIZE * GAUGE_UV_U * widthRate;
	gaugePos.y = screenHeight * GAUGE_INFO.anchorY + GAUGE_INFO.offsetY * heightRate - GAUGE_SIZE * GAUGE_UV_V * widthRate;

	renderer_ = std::make_unique<PixelRenderer>();
	renderer_->SetPos(gaugePos);
	renderer_->SetSize(Vector2(GAUGE_SIZE * widthRate, GAUGE_SIZE * widthRate));
	renderer_->MakeSquereVertex();
}

void BossUI::Draw(void)
{
	//表示するものがいないなら
	if (!boss_ || !isDraw_)return;

	//インスタンス
	auto& app = Application::GetInstance();
	float screenWidth = static_cast<float>(app.GetWindowWidth());
	float screenHeight = static_cast<float>(app.GetWindowHeight());

	//ボスUI
	float hp = boss_->GetHp();
	float hpMax = boss_->GetHpMax();
	float widthRate = screenWidth / static_cast<float>(BASE_WIDTH);
	float heightRate = screenHeight / static_cast<float>(BASE_HEIGHT);

	// 中心座標
	Vector2 healthCenter;
	healthCenter.x = screenWidth * HP_INFO.anchorX + HP_INFO.offsetX * widthRate;
	healthCenter.y = screenHeight * HP_INFO.anchorY + HP_INFO.offsetY * heightRate;

	// サイズ
	float hpWidth = HP_WIDTH * widthRate;
	float hpHeight = HP_HEIGHT * heightRate;

	// 左上・右下
	Vector2 healthPos;
	healthPos.x = healthCenter.x - hpWidth * 0.5f;
	healthPos.y = healthCenter.y - hpHeight * 0.5f;

	Vector2 healthWH;
	healthWH.x = healthPos.x + hpWidth;
	healthWH.y = healthPos.y + hpHeight;

	// 枠
	DrawBox(
		healthPos.x - HP_WINDOW * widthRate,
		healthPos.y - HP_WINDOW * heightRate,
		healthWH.x + HP_WINDOW * widthRate,
		healthWH.y + HP_WINDOW * heightRate,
		0x000000,
		TRUE);

	// HP
	if (hp > 0.0f)
	{
		DrawBox(
			healthPos.x,
			healthPos.y,
			healthPos.x + hpWidth * (hp / hpMax),
			healthPos.y + hpHeight,
			0xff8888,
			TRUE);
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