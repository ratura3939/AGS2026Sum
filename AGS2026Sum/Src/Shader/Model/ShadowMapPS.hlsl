// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

SamplerState g_SrcSampler : register(s0);
Texture2D g_SrcTexture : register(t0);

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    //深度を設定(カメラから離れているほど1.0に近づく)
    float4 shadowDepth;
    shadowDepth.rgb = PSInput.svPos.z;
    shadowDepth.a = 1.0f;
    
    //完全に投下しているなら影を描画しない
    float4 color = g_SrcTexture.Sample(g_SrcSampler, PSInput.uv);
    if(color.a < 0.001f)
    {
        discard;
    }
    
    return shadowDepth;
}