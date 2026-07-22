// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbAura : register(b4)
{
    float auraSize;
    float chargeRate;
    float time;
    float dummy;
    float3 auraColor;
    float dummy2;
    float3 g_camera_pos; 
    float dummy3;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 N = normalize(input.normal);

    float3 V = normalize(g_camera_pos - input.worldPos);

    //----------------------------
    // リムライト
    //----------------------------
    float rim = 1.0 - saturate(dot(V, N));

    // 太さ
    rim = smoothstep(0.25, 1.0, rim);

    //----------------------------
    // ゆらゆら
    //----------------------------
    float pulse =
        0.8 + 0.2 * sin(time * 8.0);

    //----------------------------
    // 溜めるほど明るく
    //----------------------------
    float intensity =
        lerp(0.5, 2.0, chargeRate);

    float3 color =
        auraColor *
        rim *
        pulse *
        intensity;

    return float4(color, rim * 0.8);
}