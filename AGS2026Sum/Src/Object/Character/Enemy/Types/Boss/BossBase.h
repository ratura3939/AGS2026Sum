#pragma once
#include "BossBattleComponent.h"
#include "../../EnemyBase.h"

class BossBase : public EnemyBase
{
public:

	//コンストラクタ
	BossBase(const ENEMY_TYPE& _type);

	//デストラクタ
	virtual ~BossBase(void)override;

	//描画
	virtual void Draw(void)override;

	//戦闘情報を返す
	BossBattleComponent& GetBattleComponent(void) { return battle_; }

	//現在HPの取得
	const float GetHp(void)const { return hp_; }
	
	//最大HPの取得
	const float GetHpMax(void)const { return hpMax_; }

	//ガードブレイク値
	const float GetGuardDurability(void)const { return battle_.GetGuardDurability(); }

protected:

	//ボスの戦闘情報
	BossBattleComponent battle_;

	//アウトラインマテリアル
	std::unique_ptr<ModelMaterial>outlineMaterial_;

	//リムライトマテリアル
	std::unique_ptr<ModelMaterial>rimLightMaterial_;

	//リムライト用
	float rimCnt_;

	//読み込み
	virtual void DoLoad(void)override;

	//シェーダーの読み込み
	virtual void LoadShader(void)override;

	//初期化
	virtual void DoInit(void)override;

	//更新
	virtual void DoUpdate(void)override;

	//コライダの生成
	virtual void CreateCollider(void)override;
};