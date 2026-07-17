#pragma once
#include<memory>

class BossBase;
class PixelMaterial;
class PixelRenderer;

class BossUI
{
public:

	//コンストラクタ
	BossUI(void);

	//デストラクタ
	~BossUI(void);

	//読み込み
	void Load(void);

	//描画
	void Draw(void);

	//UI表示するボスの設定
	void SetDrawBoss(const BossBase* _boss);

private:

	//シェーダーの定数バッファの数
	static constexpr int CONST_BUFF_NUM = 2;

	//ゲージの位置
	static constexpr float GAUGE_POS_X = 100.0f;
	static constexpr float GAUGE_POS_Y = 200.0f;
	static constexpr float GAUGE_RADIUS = 30.0f;

	//HPバーの位置
	static constexpr int HP_LOCAL_POS_X = -100;
	static constexpr int HP_LOCAL_POS_Y = -200;
	static constexpr int HP_WIDTH = 500;
	static constexpr int HP_HEIGHT = 20;
	static constexpr int HP_WINDOW = 5;

	//表示するボス
	const BossBase* boss_;

	//シェーダー
	std::unique_ptr<PixelMaterial> material_;
	std::unique_ptr<PixelRenderer> renderer_;
};