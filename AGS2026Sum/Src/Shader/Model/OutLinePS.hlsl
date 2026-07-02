// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float3 g_camera_pos;
    float dummy_camera;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color;
    
    //テクスチャーの色を取得
    color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
    
    float3 cameraRay = normalize(g_camera_pos - PSInput.worldPos);
    float dotF = abs(dot(PSInput.normal, cameraRay));
    
    const float threshold = 0.1f; // しきい値を設定
    
    if(dotF < threshold)
    {
        // カメラ方向と法線の内積がしきい値未満の場合、アウトライン色を適用
        color = g_color;
    }
    
    return color;
}