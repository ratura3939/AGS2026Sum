// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 定数バッファ：スロット4番目(b4)
cbuffer cbNormalDepthParam : register(b4)
{
    float g_farClip; //深度を正規化用の基準距離
    float3 dummy_normalDepth;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    // 法線を正規化(補間でズレた長さを補正)
    float3 normal = normalize(PSInput.normal);

    // -1〜1の法線を0〜1範囲に変換してRGBへ格納
    float3 packedNormal = normal * 0.5f + 0.5f;

    // ビュー空間Z値(vwPos.z)を0〜1に正規化して深度として格納
    // vwPos.zはカメラからの距離(手前が小さい値)想定
    float depth = saturate(PSInput.vwPos.z / g_farClip);

    return float4(packedNormal, depth);
}