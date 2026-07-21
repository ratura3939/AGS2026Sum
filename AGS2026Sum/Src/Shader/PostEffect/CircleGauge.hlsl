#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float2 center; // 中心UV
    float progress; // 0～1 回転ゲージ
    float cnt; // カウンタ
    float radius; // 半径
    float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{    
    //円周率
    float PI = 3.14159265;
    float TWO_PI = PI * 2;
    
    //分割段階
    int splitNum = 6;

    //最初の色
    float4 startColor = { 1.0f, 0.0f, 0.0f, 1.0f };
    
    //最後の色
    float4 endColor = { 1.0f, 0.0f, 0.0f, 1.0f };

    //UV
    float2 uv = PSInput.uv;

    //ベクトル
    float2 dir = uv - center;
    
    //長さ
    float dist = length(dir);
    
    //円を作る
    if (dist > radius)
        discard;
        
    //角度計算（-PI～PI）
    float angle = atan2(dir.y, dir.x);
    angle = fmod(angle + TWO_PI + PI * 0.5, TWO_PI);

    //0～1 の UV と同じ範囲に正規化(反時計回り)
    float ang01 = 1.0f - angle / TWO_PI;
    
    // 6段階に丸める
    float segmentProgress = ceil(progress * splitNum) / splitNum;

    //progress判定
    if (ang01 >= segmentProgress)
        discard;

    //チャージ中に色を変える
    float4 returnColor = lerp(startColor, endColor, segmentProgress);

    //色を返す
    return returnColor;
}