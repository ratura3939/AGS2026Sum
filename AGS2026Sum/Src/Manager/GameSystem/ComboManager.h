#pragma once
#include "../../Common/Singleton.h"

class ComboManager : public Singleton<ComboManager>
{
	//シングルトン化のため、Singletonクラスをフレンドクラスに指定
	friend class Singleton<ComboManager>;

public:

	//初期化
	void Init(void)override;

	//コンボ加算
	void AddCombo(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//コンボリセット
	void ResetCombo(void);

	//ヒット数リセット
	void ResetHitCount(void) { hitCount_ = 0; }

	//コンボ数の取得
	const int GetComboCount(void)const { return comboCount_; }

	//ヒット数の取得
	const int GetHitCount(void)const { return hitCount_; }

private:

	//コンボUIの描画
	void DrawComboUI(void);

	//コンボの最大受付時間
	static constexpr float COMBO_MAX_TIME = 2.0f;

	//コンボ数
	int comboCount_;

	//コンボタイマー
	float comboTimer_;

	//ヒット数
	int hitCount_;

	//数字画像
	int* numberImages_;

	//「COMBO」画像
	int comboStringImage_;

	//コンストラクタ
	ComboManager(void);

	//デストラクタ
	~ComboManager(void)override;
};

