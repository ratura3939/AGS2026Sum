
float ShadowCalc(float3 lightAtPos, Texture2D tex, SamplerState texSampler, float3 normal, float3 lightDir, float bias)
{
    //ブラーの大きさ
    float blurScale = 2.0f;
    
    //光から深度UVに変換
    float2 depthUV;
    depthUV.x = (lightAtPos.x + 1.0f) / 2.0f;
    depthUV.y = 1.0f - (lightAtPos.y + 1.0f) / 2.0f;
    
    //シャドウマップの深度
    float shadowDepth = tex.Sample(texSampler, depthUV).r;
    
    //深度比較
    return (lightAtPos.z > shadowDepth + bias) ? 0.5f : 1.0f;
}