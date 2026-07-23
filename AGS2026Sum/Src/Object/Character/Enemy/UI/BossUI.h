#pragma once
#include<memory>
#include"../../Attack/AttackDataBase.h"

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

	//UIの解像度
	static constexpr int BASE_WIDTH = 1920;
	static constexpr int BASE_HEIGHT = 1080;

	//ゲージの位置
	static constexpr UIInfo GAUGE_INFO = { 0.75f,0.1f, 0.0f,0.0f };
	static constexpr float GAUGE_UV_U = 0.5f;
	static constexpr float GAUGE_UV_V = 0.5f;
	static constexpr float GAUGE_RADIUS = 0.2f;
	static constexpr int GAUGE_SIZE = 300;

	//HPバーの位置
	static constexpr UIInfo HP_INFO = { 0.5f,0.1f, 0.0f,0.0f };
	static constexpr int HP_WIDTH = 700;
	static constexpr int HP_HEIGHT = 15;
	static constexpr int HP_WINDOW = 2;

	//アイコンの相対座標
	static constexpr int ICON_LOCAL_POS_X = 0;
	static constexpr int ICON_LOCAL_POS_Y = 80;
	static constexpr float ICON_SIZE = 0.4f;
	static constexpr float ICON_ZOOM_MIN = 0.4f;
	static constexpr float ICON_ZOOM_MAX = 0.5f;
	static constexpr float ICON_ZOOM_SPEED = 0.01f;

	//表示するボス
	const BossBase* boss_;

	//描画の有無
	bool isDraw_;

	//HP情報
	Vector2 healthPos_;
	Vector2 healthWH_;
	float widthRate_;
	float heightRate_;

	//攻撃アイコン
	std::array<int, static_cast<int>(AttackDataBase::ATTACK_ELEMENT::MAX)> atkIcon_;

	//シェーダー
	std::unique_ptr<PixelMaterial> material_;
	std::unique_ptr<PixelRenderer> renderer_;
};