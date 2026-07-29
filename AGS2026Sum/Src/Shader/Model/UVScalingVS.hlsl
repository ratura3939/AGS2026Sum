#include "../Common/VertexToPixelHeader.hlsli"
// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME
// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

cbuffer cbPram:register(b7) {
	float2 g_scale;
	float2 scl_dummy;
}

// シャドウマップ用定数バッファ：スロット8番目
cbuffer cbParamShadow : register(b8)
{
    float4x4 g_light_viewmatrix;
    float4x4 g_light_projectionMatrix;
};

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;
	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;

	// float3 → float4
	lLocalPosition.xyz = VSInput.pos;
	lLocalPosition.w = 1.0f;

	// ローカル座標をワールド座標に変換(剛体)
	lWorldPosition.w = 1.0f;
	//引数①を引数②の型に変換する
	lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);	//←localWorldMatrixこれらはDXLIBにあるやつ

	// ワールド座標をビュー座標に変換
	lViewPosition.w = 1.0f;
	//引数①を引数②の型に変換する
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos.xyz = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

	//UVスケーリング
	ret.uv.x = VSInput.uv0.x * g_scale.x;
	ret.uv.y = VSInput.uv0.y * g_scale.y;

	// ライトのビュー座標を射影座標に変換
    float4 lLViewPosition = mul(g_light_viewmatrix, lWorldPosition);
    ret.lightAtPos = mul(g_light_projectionMatrix, lLViewPosition).xyz;

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
	// 出力パラメータを返す
	return ret;
}