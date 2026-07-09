#pragma once
#include "BossBattleComponent.h"
#include "../../EnemyBase.h"

class MiddleBoss : public EnemyBase
{
public:

	//コンストラクタ
	MiddleBoss(void);

	//デストラクタ
	~MiddleBoss(void)override;

	//描画
	void Draw(void)override;

	//戦闘情報を返す
	BossBattleComponent& GetBattleComponent(void) { return battle_; }

	//コライダの生成
	virtual void CreateCollider(void);

private:

	//HPバーの位置
	static constexpr int HP_LOCAL_POS_X = -100;
	static constexpr int HP_LOCAL_POS_Y = -200;
	static constexpr int HP_WIDTH = 500;
	static constexpr int HP_HEIGHT = 20;
	static constexpr int HP_WINDOW = 5;

	//ボスの戦闘情報
	BossBattleComponent battle_;

	//アウトラインマテリアル
	std::unique_ptr<ModelMaterial>outlineMaterial_;	

	//読み込み
	void DoLoad(void)override;

	//シェーダーの読み込み
	void LoadShader(void)override;

	//初期化
	void DoInit(void)override;

	//更新
	void DoUpdate(void)override;
};

