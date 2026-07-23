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
    
    //枠の色
    float4 frameColor = { 0.0f, 0.0f, 0.0f, 1.0f };

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

    //ゲージの割合の仕切り
    float cell = ang01 * splitNum;
    float local = frac(cell);

    float w = fwidth(local);

    if (local < w * 1.0f || local > 1.0f - w * 1.0f)
    {
        return frameColor;
    }
    
    //ゲージの枠
    float r = dist / radius;
    if (r > 0.90f)
    {
        return frameColor;
    }

    //progress判定
    if (ang01 >= segmentProgress)
        return float4(0.0f,0.0f,0.0f,1.0f);
    
    // このピクセルが属するセグメント番号
    float segment = floor(ang01 * splitNum);
                
    // セグメント内の進行度（0～1）
    float segmentFloat = segmentProgress * splitNum;
    // 削れた割合
    float damageProgress = (1.0f - progress) * splitNum;
    // 今削っているセグメント
    float currentSegment = floor(damageProgress);
    // セグメント内の進行度
    float segmentRate = frac(damageProgress);
    //割合で色変え
    float4 returnColor = lerp(startColor, endColor, segmentProgress);

    //ひびが入るタイミング
    float crackStart = 0.5f;

    //現在ゲージにひび
    if (segment == splitNum - 1 - currentSegment &&
    segmentRate > crackStart)
    {
        //ひびのUV
        float2 crackUV;
        float scaleX = 0.5f;
        crackUV.x = (frac(ang01 * splitNum) - 0.5f) * scaleX + 0.5f;
        crackUV.y = dist / radius;
        crackUV.x = saturate(crackUV.x);
        
        //ひび画像
        float4 crack = tex.Sample(texSampler, crackUV);
        
        // ヒビを重ねる
        returnColor.rgb = lerp(returnColor.rgb, float3(0, 0, 0), crack.a);
    }
    
    //色を返す
    return returnColor;
}