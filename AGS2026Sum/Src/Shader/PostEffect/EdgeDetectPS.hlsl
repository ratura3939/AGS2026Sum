#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 法線・深度バッファ(RGB=法線、A=深度)
Texture2D normalDepthTexture : register(t0);
SamplerState normalDepthSampler : register(s0);

//エッジ検出パラメータ
cbuffer cbEdgeParam : register(b4)
{
    float4 g_outlineColor;      //アウトラインの色
    float2 g_texelSize;         //1ピクセル分のUVサイズ(1/画面幅, 1/画面高さ)
    float g_depthThreshold;     //深度差のしきい値
    float g_normalThreshold;    //法線差のしきい値
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;

    //中心と上下左右のサンプリング
    float4 center = normalDepthTexture.Sample(normalDepthSampler, uv);
    float4 left = normalDepthTexture.Sample(normalDepthSampler, uv - float2(g_texelSize.x, 0.0f));
    float4 right = normalDepthTexture.Sample(normalDepthSampler, uv + float2(g_texelSize.x, 0.0f));
    float4 up = normalDepthTexture.Sample(normalDepthSampler, uv - float2(0.0f, g_texelSize.y));
    float4 down = normalDepthTexture.Sample(normalDepthSampler, uv + float2(0.0f, g_texelSize.y));

    //法線差(RGB)の合計
    float normalDiff =
        length(center.rgb - left.rgb) +
        length(center.rgb - right.rgb) +
        length(center.rgb - up.rgb) +
        length(center.rgb - down.rgb);

    //深度差(A)の合計
    float depthDiff =
        abs(center.a - left.a) +
        abs(center.a - right.a) +
        abs(center.a - up.a) +
        abs(center.a - down.a);

    //どちらかがしきい値を超えたらエッジと判定
    bool isEdge = (normalDiff > g_normalThreshold) || (depthDiff > g_depthThreshold);

    if (!isEdge)
    {
       // エッジではなため透明
        discard;
    }

     //エッジのためアウトラインカラーを返す
    return g_outlineColor;
}