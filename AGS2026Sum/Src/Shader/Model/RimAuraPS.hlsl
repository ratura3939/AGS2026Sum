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

    float3 V =
    normalize(g_camera_pos - input.worldPos);

    float rim =
    1.0 - saturate(dot(V, N));

    rim = smoothstep(0.2, 1.0, rim);

    float3 color =
    float3(1, 0, 0) * rim;

    return float4(color, 1);
}