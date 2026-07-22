// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; //基本色

    float4 g_ambient_color; //環境光

    float3 g_camera_pos; //視線位置

    float g_rimStrength; //リムライトの強度
    float3 g_rimColor; //リムライトの色
    float g_rimPower; //リムライトの幅
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    //==========================
    // テクスチャ
    //==========================
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

    if (color.a < 0.01f)
    {
        discard;
    }
    
    //==========================
    // 法線・視線
    //==========================
    float3 normal = normalize(PSInput.normal);
    float3 viewDir = normalize(PSInput.worldPos - g_camera_pos);

    //==========================
    // リムライト
    //==========================
    float rim = 1.0f - saturate(dot(normal, viewDir));
    rim = pow(rim, 8.0f);

    float3 rimLight =
    rim *
    g_rimColor *
    g_rimStrength;

    //==========================
    // 最終色
    //==========================
    float3 rgb =
        color.rgb * g_color.rgb +
        rimLight +
        g_ambient_color.rgb;

    return float4(rgb, color.a);
}