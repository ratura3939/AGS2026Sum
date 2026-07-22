#pragma once
#include<memory>

class EnemyBase;
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
	void SetDrawBoss(const EnemyBase* _boss);

private:

	//UI用情報
	struct UIInfo
	{
		float anchorX = 0.0f;
		float anchorY = 0.0f;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
	};

	//シェーダーの定数バッファの数
	static constexpr int CONST_BUFF_NUM = 2;

	//ゲージの位置
	static constexpr float GAUGE_UV_U = 0.5f;
	static constexpr float GAUGE_UV_V = 0.2f;
	static constexpr float GAUGE_RADIUS = 0.1f;

	//HPバーの位置
	static constexpr UIInfo HP_INFO = { 0.5f,0.0f, 0.0f,50.0f };
	static constexpr int HP_WIDTH = 500;
	static constexpr int HP_HEIGHT = 10;
	static constexpr int HP_WINDOW = 2;

	//表示するボス
	const BossBase* boss_;

	//描画の有無
	bool isDraw_;

	//シェーダー
	std::unique_ptr<PixelMaterial> material_;
	std::unique_ptr<PixelRenderer> renderer_;
};