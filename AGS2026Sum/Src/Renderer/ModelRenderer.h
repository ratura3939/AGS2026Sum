#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/ModelMaterial.h"

class ModelRenderer
{
public:

	// 頂点シェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	// 頂点シェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS_MATRIX = 8;

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// コンストラクタ
	ModelRenderer(void);

	// デストラクタ
	~ModelRenderer(void);

	// 描画
	void Draw(int _modelId, ModelMaterial& _modelMaterial);

private:
	// シェーダ設定(頂点)
	void SetReserveVS(ModelMaterial& _modelMaterial);

	// シェーダ設定(ピクセル)
	void SetReservePS(ModelMaterial& _modelMaterial);

};
