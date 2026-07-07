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

cbuffer OutlineParam : register(b7)
{
    float g_outlineWidth; // 押し出し幅(ワールド空間基準)
    float g_distanceScale; // 視距離補正の強さ(0で補正なし)
    float2 outline_dummy; 
    float3 g_cameraPosition; //カメラ座標(ワールド空間)
    float camera_dummy;
};

VS_OUTPUT main(VS_INPUT VSInput)
{
	VS_OUTPUT ret;

	// スキニング用のローカル→ワールド変換行列を作成+++( 開始 )
    int4 lBoneIdx;
    float4 lL_W_Mat[3];
    float4 lWeight;

	// BONE4
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

	// BONE8
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
	// スキニング用のローカル→ワールド変換行列を作成+++( 終了 )


	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    float3 lWorldNormal;

	// float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;

	// ローカル座標をワールド座標に変換(スキニング適用後)
    lWorldPosition.w = 1.0f;
    lWorldPosition.x = dot(lLocalPosition, lL_W_Mat[0]);
    lWorldPosition.y = dot(lLocalPosition, lL_W_Mat[1]);
    lWorldPosition.z = dot(lLocalPosition, lL_W_Mat[2]);

	// 法線もスキニング後のワールド法線を使用
    lWorldNormal.x = float(dot(VSInput.norm, lL_W_Mat[0]));
    lWorldNormal.y = float(dot(VSInput.norm, lL_W_Mat[1]));
    lWorldNormal.z = float(dot(VSInput.norm, lL_W_Mat[2]));
    lWorldNormal = normalize(lWorldNormal);

	// カメラからの距離を仮算出(押し出し量の補正用)
	// g_base.cameraPositionが無い場合はviewMatrixの逆行列や
	// 別途渡しているカメラ座標定数に置き換えてください
    float lCameraDistance = length(lWorldPosition.xyz - g_cameraPosition);

	// 距離に応じて押し出し量をスケーリング
	// g_distanceScaleが0なら実質固定値と同じ
    float lFinalWidth = g_outlineWidth * (1.0f + lCameraDistance * g_distanceScale);

	// インバーテッドハル法:ワールド法線方向に押し出す
    lWorldPosition.xyz += lWorldNormal * lFinalWidth;

    ret.worldPos = lWorldPosition.xyz;

	// ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);


	// その他、ピクセルシェーダへ渡す値 ++++++++++++( 開始 )
    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;

    ret.normal = lWorldNormal;
    ret.diffuse = VSInput.diffuse;
    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
    ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
	// その他、ピクセルシェーダへ渡す値 ++++++++++++( 終了 )


	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
    return ret;
}