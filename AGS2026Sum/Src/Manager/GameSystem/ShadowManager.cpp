#include "../../pch.h"
#include "../../Application.h"
#include "../Generic/SceneManager.h"
#include "../Generic/Camera.h"
#include "../Generic/ResourceManager.h"
#include "ShadowManager.h"

void ShadowManager::Load(void)
{
	//シャドウマップ用のシェーダーハンドル
	auto& res = ResourceManager::GetInstance();
	shadowPS_ = LoadPixelShader((Application::PATH_SHADER + L"ShadowMapPS.cso").c_str());
	shadowMeshVS_ = LoadVertexShader((Application::PATH_SHADER + L"ShadowMapMeshVS.cso").c_str());
	shadowSkinnedMeshVS_ = LoadVertexShader((Application::PATH_SHADER + L"ShadowMapSkinnedMeshVS.cso").c_str());
}

void ShadowManager::Init(void)
{
	//描画先テクスチャをシャドウマップ用に変更(グレースケールなので1)
	SetCreateDrawValidGraphChannelNum(SHADOW_CHANNEL_NUM);

	//描画先テクスチャを深度の値が精度の高いfloat型で作成する
	SetDrawValidFloatTypeGraphCreateFlag(true);

	//描画先テクスチャの色深度を設定
	SetCreateGraphColorBitDepth(COLOR_BIT_DEPTH);

	//シャドウマップ用テクスチャの作成
	shadowTex_ = MakeScreen(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, false);

	//描画先テクスチャの設定を元に戻す
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(DEFAULT_CHANNEL_NUM);
	SetCreateGraphChannelBitDepth(0);
}

void ShadowManager::Draw(void)
{
	//シャドウマップ用テクスチャに描画
	SetDrawScreen(shadowTex_);

	//シャドウマップ用テクスチャを白色にクリア
	SetBackgroundColor(COLOR_MAX, COLOR_MAX, COLOR_MAX);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);

	//カメラ
	auto& scnMng = SceneManager::GetInstance();
	auto& camera = scnMng.GetCamera();
	
	//シャドウカメラの設定
	camera.CameraSettingShadow();

	// 設定したカメラのビュー行列と射影行列を取得しておく
	lightViewMat_ = GetCameraViewMatrix();
	lightProjectionMat_ = GetCameraProjectionMatrix();

	//オリジナルシェーダー使用の設定
	MV1SetUseOrigShader(true);

	//剛体メッシュ描画用の頂点シェーダー
	SetUseVertexShader(shadowMeshVS_);

	//ピクセルシェーダー
	SetUsePixelShader(shadowPS_);

	//ボーンなしモデル
	for (auto& model : shadowModelId_[static_cast<int>(MESH_TYPE::NORMAL)])
	{
		MV1DrawModel(model);
	}

	// シェーダーのリセット
	ResetShader();

	//オリジナルシェーダー使用の再設定
	MV1SetUseOrigShader(true);

	//剛体メッシュ描画用の頂点シェーダー
	SetUseVertexShader(shadowSkinnedMeshVS_);

	//ボーン付きモデル
	for (auto& model : shadowModelId_[static_cast<int>(MESH_TYPE::SKINNED)])
	{
		MV1DrawModel(model);
	}

	// シェーダーのリセット
	ResetShader();

	//描画スクリーンをもとに戻す
	SetDrawScreen(scnMng.GetMainScreen());

	//画面の初期化
	ClearDrawScreen();

	//カメラ設定
	camera.SetBeforeDraw();
}

void ShadowManager::AddShadowModel(const MESH_TYPE& _meshType, const int _modelId)
{
	//モデル
	auto& models = shadowModelId_[static_cast<int>(_meshType)];

	//重複は無視
	models.insert(_modelId);
}

void ShadowManager::SubShadowModel(const MESH_TYPE& _meshType, const int _modelId)
{
	//モデル
	auto& models = shadowModelId_[static_cast<int>(_meshType)];

	//モデルを除く
	models.erase(_modelId);
}

ShadowManager::ShadowManager(void)
	: shadowTex_(-1)
	, shadowPS_(-1)
	, shadowMeshVS_(-1)
	, shadowSkinnedMeshVS_(-1)
	, lightViewMat_(MATRIX{})
	, lightProjectionMat_(MATRIX{})
{
}

ShadowManager::~ShadowManager(void)
{
	//テクスチャの削除
	DeleteGraph(shadowTex_);
}

void ShadowManager::ResetShader(void)
{
	//シェーダーを元に戻す
	MV1SetUseOrigShader(false);
	SetUseVertexShader(-1);
	SetUsePixelShader(-1);

	//前回使用分のテクスチャを引き継がないようにする
	SetUseTextureToShader(0, -1);
}