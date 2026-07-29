#include "../../pch.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/GameSystem/ShadowManager.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include"../Common/Geometry/Model.h"
#include "StageObjectBase.h"

namespace {
	const VECTOR INIT_POS = { -0.0f,-100.0f,-0.0f };
	const int VS_BUFF_NUM = 1;
	const int PS_BUFF_NUM = 0;
	const int BUFFER_MATRIX_SIZE = 2;

	const float SCALING_X = 1.0f;
	const float SCALING_Y = 1.0f;

	const FLOAT4 SCALING_UV = { SCALING_X,SCALING_Y,0.0f,0.0f };

	const VECTOR INIT_SCALE_FOR_TEST = { 3.0f,3.0f,3.0f };
}

StageObjectBase::StageObjectBase(int _modelId, const VECTOR& _position)
{
	modelId_ = _modelId;
	pos_ = _position;
}

StageObjectBase::~StageObjectBase(void)
{
	//影を削除
	//ShadowManager::GetInstance().SubShadowModel(ShadowManager::MESH_TYPE::NORMAL, modelId_);
}

void StageObjectBase::Draw(void)
{
	//影マネージャー
	auto& shadow = ShadowManager::GetInstance();

	// マトリックスバッファーの設定
	material_->SetConstBufVSMatrix(0, shadow.GetLightViewMatrix());
	material_->SetConstBufVSMatrix(1, shadow.GetLightProjectionMatrix());

	// シャドウマップの設定
	material_->SetTextureBuf(ModelRenderer::CONSTANT_BUF_SLOT_BEGIN_VS_MATRIX, shadow.GetShadowTexture());

	renderer_->Draw(modelId_,*material_);
}

void StageObjectBase::Release(void)
{
}

void StageObjectBase::HitCollider(std::weak_ptr<Collider> _col)
{
}

void StageObjectBase::DoLoad(void)
{
	//コライダ
	std::unique_ptr<Geometry> geo = std::make_unique<Model>(pos_, pos_, quaRot_, 20000.0f, modelId_);
	MakeCollider(std::move(geo), Collider::COL_TAG::STAGE, { Collider::COL_TAG::PLAYER,Collider::COL_TAG::ENEMY });

	material_ = std::make_unique<ModelMaterial>(L"UVScalingVS.cso", VS_BUFF_NUM, L"StdModelPS.cso", PS_BUFF_NUM, BUFFER_MATRIX_SIZE);
	material_->AddConstBufVS(SCALING_UV);

	//影マネージャー
	auto& shadow = ShadowManager::GetInstance();

	// マトリックスバッファーの追加
	material_->AddConstBufVSMatrix(shadow.GetLightViewMatrix());
	material_->AddConstBufVSMatrix(shadow.GetLightProjectionMatrix());

	//影を追加
	//shadow.AddShadowModel(ShadowManager::MESH_TYPE::NORMAL, modelId_);

	// シャドウマップの設定
	material_->SetTextureBuf(ModelRenderer::CONSTANT_BUF_SLOT_BEGIN_VS_MATRIX, shadow.GetShadowTexture());

	scl_ = INIT_SCALE_FOR_TEST;

	renderer_ = std::make_unique<ModelRenderer>();
}

void StageObjectBase::DoInit(void)
{
}

void StageObjectBase::DoUpdate(void)
{
}
