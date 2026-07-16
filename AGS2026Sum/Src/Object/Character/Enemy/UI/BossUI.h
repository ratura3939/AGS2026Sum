#pragma once

class BossBase;

class BossUI
{
public:

	//コンストラクタ
	BossUI(void);

	//デストラクタ
	~BossUI(void);

	//描画
	void Draw(void);

	//UI表示するボスの設定
	void SetDrawBoss(const BossBase* _boss);

private:

	//HPバーの位置
	static constexpr int HP_LOCAL_POS_X = -100;
	static constexpr int HP_LOCAL_POS_Y = -200;
	static constexpr int HP_WIDTH = 500;
	static constexpr int HP_HEIGHT = 20;
	static constexpr int HP_WINDOW = 5;

	//表示するボス
	const BossBase* boss_;
};