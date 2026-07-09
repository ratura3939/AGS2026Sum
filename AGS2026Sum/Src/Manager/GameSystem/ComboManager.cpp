#include "../../pch.h"
#include "../Generic/SceneManager.h"
#include "../Decoration/UIManager2d.h"
#include"../Generic/ResourceManager.h"
#include "ComboManager.h"

void ComboManager::Init(void)
{
	//初期化
	ResetCombo();
	ResetHitCount();

	ResourceManager& resM = ResourceManager::GetInstance();
	using SOURCE = ResourceManager::SRC;

	numberImages_ = resM.Load(SOURCE::COMBO_NUMBER_IMG).handleIds_;
	comboStringImage_ = resM.Load(SOURCE::COMBO_STRING_IMG).handleId_;
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

	//コンボカウンター描画
	DrawComboUI();

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

void ComboManager::DrawComboUI(void)
{
	//コンボ数が存在しない場合処理しない
	if (comboCount_ <= 0) {
		return;
	}

	const int drawPos_Y = 120;			//描画位置Y(共通)
	const int comboStartPos_X = 100;	//「COMBO」描画位置X
	const int numberPosOffset_X = 20;	//数字ごとの位置調整差分

	const double exRate = 1.0;			//拡大率
	const double angle = 0.0;			//回転

	//「COMBO」の表示
	DrawRotaGraph(comboStartPos_X, drawPos_Y, exRate, angle, comboStringImage_, false);

	std::vector<int>digitNumbers;	//桁それぞれを保持
	int remainingCombo = comboCount_;

	while (remainingCombo > 0) {
		//一の位を取得
		int useNumber = remainingCombo % 10;

		//下位桁から順に保存
		digitNumbers.push_back(useNumber);

		//一桁分削る
		remainingCombo /= 10;
	}

	//描画初期位置
	int numberDrawPos_X = comboStartPos_X - numberPosOffset_X;

	for (int& useNumber : digitNumbers) {
		//数字の描画
		DrawRotaGraph(numberDrawPos_X, drawPos_Y, exRate, angle, numberImages_[useNumber], false);

		//位置差分
		numberDrawPos_X -= numberPosOffset_X;
	}
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
