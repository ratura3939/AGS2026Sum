// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
#include "../Common/Pixel/PixelShaderUtility.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//// シャドウマップの有効フラグ
//#define SHADOWMAP 8

SamplerState shadowMap0Sampler : register(s8); // シャドウマップ０テクスチャ
Texture2D shadowMap0Texture : register(t8);

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;

    float3 g_spot_light_dir; // スポットライト方向
    float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	//return g_color;
	float4 color;

	//テクスチャーの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler,PSInput.uv);
	
	//影の影響力を取得
    float shadowFactor = ShadowCalc(PSInput.lightAtPos, shadowMap0Texture, shadowMap0Sampler, PSInput.normal, g_spot_light_dir, 0.001f);
	
	//影の反映
    color.rgb *= shadowFactor;
		
	//関数の戻り値がラスタライザに渡される
	return color;
}