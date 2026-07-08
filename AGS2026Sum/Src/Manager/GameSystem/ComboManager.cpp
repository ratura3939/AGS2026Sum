#include "../../pch.h"
#include "../Generic/SceneManager.h"
#include "../Decoration/UIManager2d.h"
#include "ComboManager.h"

void ComboManager::Init(void)
{
	//初期化
	ResetCombo();
	ResetHitCount();
}

void ComboManager::AddCombo(void)
{
	//コンボ数を加算
	comboCount_++;

	//ヒット数を加算
	hitCount_++;

	//コンボタイマーをリセット
	comboTimer_ = 0.0f;
}

void ComboManager::Update(void)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//コンボタイマーを更新
	comboTimer_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

	//コンボタイマーが最大受付時間を超えた場合
	if (comboTimer_ > COMBO_MAX_TIME)
	{
		//コンボをリセット
		ResetCombo();
	}
}

void ComboManager::Draw(void)
{
	//コンボ数が0以上なら
	if (comboCount_ > 0)
	{
		//コンボ数を描画
		DrawString(10, 120, (L"Combo: " + std::to_wstring(comboCount_)).c_str(), 0xffffff);
	}

	//ヒット数の描画
	DrawString(10, 150, (L"Hit: " + std::to_wstring(hitCount_)).c_str(), 0xffffff);
}

void ComboManager::ResetCombo(void)
{
	//コンボ数をリセット
	comboCount_ = 0;

	//コンボタイマーをリセット
	comboTimer_ = 0.0f;
}

ComboManager::ComboManager(void)
	: comboCount_(0)
	, comboTimer_(0.0f)
	, hitCount_(0)
{
}

ComboManager::~ComboManager(void)
{
}
