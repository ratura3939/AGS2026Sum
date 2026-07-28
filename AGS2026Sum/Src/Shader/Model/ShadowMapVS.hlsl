//スキンメッシュ対応
// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_8FRAME 

#define L_W_MAT g_localWorldMatrix.lwMatrix

// OUT
#define VS_OUTPUT VertexToPixelShadow
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

VS_OUTPUT main(VS_INPUT VSInput)
{
VS_OUTPUT ret;

    //------------------------------------------------------------
    // スキニング
    //------------------------------------------------------------

    //スキンメッシュ用のローカルからワールド座標の変換行列を作成
    int4 lBoneIdx;
    float4 lL_W_Mat[3];
    float4 lWeight;

    lBoneIdx = VSInput.blendIndices0;
    lWeight = VSInput.blendWeight0;

    lL_W_Mat[0] = L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
    lL_W_Mat[1] = L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
    lL_W_Mat[2] = L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;

    lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;

    lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;

    lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;

    // Bone8
    lBoneIdx = VSInput.blendIndices1;
    lWeight = VSInput.blendWeight1;

    lL_W_Mat[0] += L_W_MAT[lBoneIdx.x + 0] * lWeight.xxxx;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.x + 1] * lWeight.xxxx;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.x + 2] * lWeight.xxxx;

    lL_W_Mat[0] += L_W_MAT[lBoneIdx.y + 0] * lWeight.yyyy;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.y + 1] * lWeight.yyyy;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.y + 2] * lWeight.yyyy;

    lL_W_Mat[0] += L_W_MAT[lBoneIdx.z + 0] * lWeight.zzzz;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.z + 1] * lWeight.zzzz;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.z + 2] * lWeight.zzzz;

    lL_W_Mat[0] += L_W_MAT[lBoneIdx.w + 0] * lWeight.wwww;
    lL_W_Mat[1] += L_W_MAT[lBoneIdx.w + 1] * lWeight.wwww;
    lL_W_Mat[2] += L_W_MAT[lBoneIdx.w + 2] * lWeight.wwww;

    //頂点座標変換
    float4 localPos;
    float4 worldPos;
    float4 viewPos;
    float3 worldNormal;
    float3 localNormal = VSInput.normal;
    
    //相対座標
    localPos = float4(VSInput.pos, 1.0f);

    //頂点スキニング
    worldPos.x = dot(localPos, lL_W_Mat[0]);
    worldPos.y = dot(localPos, lL_W_Mat[1]);
    worldPos.z = dot(localPos, lL_W_Mat[2]);
    worldPos.w = 1.0f;

    //法線スキニング
    worldNormal.x = dot(float4(localNormal, 0.0f), lL_W_Mat[0]);
    worldNormal.y = dot(float4(localNormal, 0.0f), lL_W_Mat[1]);
    worldNormal.z = dot(float4(localNormal, 0.0f), lL_W_Mat[2]);
    worldNormal = normalize(worldNormal);
    
    //ワールド座標をビュー座標に変換
    viewPos.xyz = mul(worldPos, g_base.viewMatrix);
    viewPos.w = 1.0f;

    //ビュー座標を射影座標に変換
    ret.svPos = mul(viewPos, g_base.projectionMatrix);
    ret.svPosRead = ret.svPos;
    
    //法線をローカルからワールドに変換
    ret.normal = normalize(mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));

    //UV
    ret.uv.xy = VSInput.uv0.xy;
    
    //ビュー座標
    ret.vwPos.xyz = viewPos.xyz;

    //出力
    return ret;
}