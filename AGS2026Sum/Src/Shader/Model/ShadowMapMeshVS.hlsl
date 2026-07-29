// VS/PS共通
#define VertexToPixelShadow_DEFINE
#include "../Common/VertexToPixelHeader.hlsli"
 
// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME
 
// OUT
#define VS_OUTPUT VertexToPixelShadow
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;
    
    //頂点座標変換
    float4 lLocalPos;
    float4 lWorldPos;
    float4 lViewPos;
    
    //相対座標
    lLocalPos = float4(VSInput.pos, 1.0f);
    
    //ローカル座標をワールド座標に変換
    lWorldPos.xyz = mul(lLocalPos, g_base.localWorldMatrix);
    lWorldPos.w = 1.0f;
    
    //ワールド座標をビュー座標に変換
    lViewPos.xyz = mul(lWorldPos, g_base.viewMatrix);
    lViewPos.w = 1.0f;
    
    //ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPos, g_base.projectionMatrix);
    ret.svPosRead = ret.svPos;
    
    //法線をローカルからワールドに変換
    ret.normal = normalize(mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));

    //UV
    ret.uv.xy = VSInput.uv0.xy;
    
    //出力
    return ret;
}