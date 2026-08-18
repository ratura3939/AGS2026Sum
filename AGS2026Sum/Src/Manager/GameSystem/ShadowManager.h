#pragma once
#include<unordered_set>
#include "../Common/Singleton.h"

class ShadowManager : public Singleton<ShadowManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<ShadowManager>;

public:

	//メッシュの種類
	enum class MESH_TYPE
	{
		NORMAL		//通常
		,SKINNED	//ボーン付き
		,MAX
	};

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//描画
	void Draw(void);

	//光のビュー行列の取得
	const MATRIX& GetLightViewMatrix(void)const { return lightViewMat_; }

	//光のビュー行列設定
	void SetLightViewMatrix(const MATRIX& _mat) { lightViewMat_ = _mat; }

	//光のプロジェクション行列の取得
	const MATRIX& GetLightProjectionMatrix(void)const { return lightProjectionMat_; }

	//光のプロジェクション行列設定
	void SetLightProjectionMatrix(const MATRIX& _mat) { lightProjectionMat_ = _mat; }

	//シャドウマップ用のテクスチャの取得
	const int GetShadowTexture(void)const { return shadowTex_; }

	/// <summary>
	/// 影描画するモデルの追加
	/// </summary>
	/// <param name="_meshType">メッシュの種類</param>
	/// <param name="_modelId">モデルID</param>
	void AddShadowModel(const MESH_TYPE& _meshType, const int _modelId);

	/// <summary>
	/// 影描画するモデルの削除
	/// </summary>
	/// <param name="_meshType">メッシュの種類</param>
	/// <param name="_modelId">モデルID</param>
	void SubShadowModel(const MESH_TYPE& _meshType, const int _modelId);

private:

	//描画先テクスチャの色深度用
	static constexpr int COLOR_BIT_DEPTH = 24;

	//シャドウマップサイズ
	static constexpr int SHADOW_MAP_SIZE = 16000;

	//シャドウマップ用チャンネル数(グレースケール)
	static constexpr int SHADOW_CHANNEL_NUM = 1;

	//通常のチャンネル数(RGBA)
	static constexpr int DEFAULT_CHANNEL_NUM = 4;

	//色値の最大
	static constexpr int COLOR_MAX = 255;

	//シャドウマップ用テクスチャ
	int shadowTex_;

	//シェーダー
	int shadowPS_;
	int shadowMeshVS_;
	int shadowSkinnedMeshVS_;

	//光のビュー行列
	MATRIX lightViewMat_;

	//プロジェクション行列
	MATRIX lightProjectionMat_;

	//影を描画するモデルID
	std::array<std::unordered_set<int>, static_cast<int>(MESH_TYPE::MAX)> shadowModelId_;

	//コンストラクタ
	ShadowManager(void);

	//デストラクタ
	~ShadowManager(void);

	//シェーダー設定の初期化
	void ResetShader(void);
};