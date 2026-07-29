#pragma once
#include <string>
#include <vector>
#include <map>
#include <DxLib.h>

class ModelMaterial
{

public:

	// 設定できる最大のテクスチャの数
	static constexpr int MAX_TEXTURES = 4;

	//サブテクスチャ一枚目
	static constexpr int SUB_TEX_1 = 11;

	// テクスチャアドレスタイプ
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIST_NOISE,
		MAX
	};

	// コンストラクタ
	ModelMaterial(
		std::wstring shaderFileNameVS, int constBufFloat4SizeVS,
		std::wstring shaderFileNamePS, int constBufFloat4SizePS,
		const int constBufMatrixSizeVS = 0
	);

	// デストラクタ
	~ModelMaterial(void);

	// 定数バッファを追加
	void AddConstBufVS(const FLOAT4& contBuf);
	void AddConstBufPS(const FLOAT4& contBuf);
	void AddConstBufVSMatrix(const MATRIX& matrix);

	// 定数バッファを更新
	void SetConstBufVS(int idx, const FLOAT4& contBuf);
	void SetConstBufPS(int idx, const FLOAT4& contBuf);
	void SetConstBufVSMatrix(int idx, const MATRIX& matrix);

	// テクスチャを更新
	void SetTextureBuf(int slot, int texDiffuse);

	// シェーダ
	int GetShaderVS(void) const;
	int GetShaderPS(void) const;

	// 定数バッファハンドル
	int GetConstBufVS(void) const;
	int GetConstBufPS(void) const;
	int GetConstBufVSMatrix(void)const;

	// 定数バッファ
	const std::vector<FLOAT4>& GetConstBufsVS(void) const;
	const std::vector<FLOAT4>& GetConstBufsPS(void) const;
	const std::vector<MATRIX>& GetConstBufsVSMatrix(void)const;

	// 画像
	const std::map<int, int>& GetTextures(void) const;

	// テクスチャアドレスを取得
	TEXADDRESS GetTextureAddress(void) const;

	// テクスチャアドレスを設定
	void SetTextureAddress(TEXADDRESS texA);

private:

	// 頂点シェーダ
	int shaderVS_;

	// ピクセルシェーダ
	int shaderPS_;

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4SizeVS_;

	// 頂点定数バッファハンドル
	int constBufVS_;

	// 頂点定数バッファマトリックスの確保サイズ(MATRIXをいくつ作るか)
	int constBufMatrixSizeVs_;

	// 頂点定数バッファマトリックス用ハンドル
	int constBufVSMatrix_;

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4SizePS_;

	// ピクセル定数バッファハンドル
	int constBufPS_;

	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// 定数定数バッファ
	std::vector<FLOAT4> constBufsVS_;

	// ピクセル定数バッファ
	std::vector<FLOAT4> constBufsPS_;

	//頂点行列定数バッファ
	std::vector<MATRIX> constBufsVSMatrix_;

	// 画像
	std::map<int, int> textures_;

};
