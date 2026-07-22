//スキンメッシュ対応
// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_8FRAME 

#define L_W_MAT g_localWorldMatrix.lwMatrix

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

cbuffer cbAura : register(b4)
{
    float auraSize; // 0.01～0.05くらい
    float chargeRate; // 0～1
    float2 dummy;
};

VS_OUTPUT main(VS_INPUT VSInput)
{
VS_OUTPUT output;


    //------------------------------------
    // スキニング行列作成
    //------------------------------------
    int4 boneIdx;
    float4 weight;

    float4 boneMat[3];

    boneIdx = VSInput.blendIndices0;
    weight = VSInput.blendWeight0;

    boneMat[0] = L_W_MAT[boneIdx.x + 0] * weight.xxxx;
    boneMat[1] = L_W_MAT[boneIdx.x + 1] * weight.xxxx;
    boneMat[2] = L_W_MAT[boneIdx.x + 2] * weight.xxxx;


    boneMat[0] += L_W_MAT[boneIdx.y + 0] * weight.yyyy;
    boneMat[1] += L_W_MAT[boneIdx.y + 1] * weight.yyyy;
    boneMat[2] += L_W_MAT[boneIdx.y + 2] * weight.yyyy;


    boneMat[0] += L_W_MAT[boneIdx.z + 0] * weight.zzzz;
    boneMat[1] += L_W_MAT[boneIdx.z + 1] * weight.zzzz;
    boneMat[2] += L_W_MAT[boneIdx.z + 2] * weight.zzzz;


    boneMat[0] += L_W_MAT[boneIdx.w + 0] * weight.wwww;
    boneMat[1] += L_W_MAT[boneIdx.w + 1] * weight.wwww;
    boneMat[2] += L_W_MAT[boneIdx.w + 2] * weight.wwww;



    //------------------------------------
    // 座標変換
    //------------------------------------
    float4 localPos = float4(VSInput.pos, 1);
    float4 worldPos;

    worldPos.x = dot(localPos, boneMat[0]);
    worldPos.y = dot(localPos, boneMat[1]);
    worldPos.z = dot(localPos, boneMat[2]);
    worldPos.w = 1;

    //------------------------------------
    // 法線
    //------------------------------------
    float3 worldNormal;

    worldNormal.x = dot(VSInput.norm, boneMat[0]);
    worldNormal.y = dot(VSInput.norm, boneMat[1]);
    worldNormal.z = dot(VSInput.norm, boneMat[2]);
    worldNormal = normalize(worldNormal);

    //------------------------------------
    // オーラ膨張
    //------------------------------------
    float size =
        lerp(auraSize, auraSize * 2.0f, chargeRate);

    worldPos.xyz += worldNormal * size;

    //------------------------------------
    // 出力
    //------------------------------------
    output.worldPos = worldPos;
    output.normal = worldNormal;


    float4 viewPos =
        mul(worldPos, g_base.viewMatrix);


    output.vwPos = viewPos;

    output.svPos =
        mul(viewPos, g_base.projectionMatrix);


    output.uv = VSInput.uv0;
    output.diffuse = VSInput.diffuse;

    output.lightDir = 0;
    output.lightAtPos = 0;


    return output;
}