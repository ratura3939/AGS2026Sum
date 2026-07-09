#include "../../pch.h"
#include "../../Manager/Generic/ResourceManager.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include"../Common/Geometry/Model.h"
#include "StageObjectBase.h"

namespace {
	const VECTOR INIT_POS = { -0.0f,-100.0f,-0.0f };
	const int VS_BUFF_NUM = 1;
	const int PS_BUFF_NUM = 0;

	const float SCALING_X = 1.0f;
	const float SCALING_Y = 1.0f;

	const FLOAT4 SCALING_UV = { SCALING_X,SCALING_Y,0.0f,0.0f };

	const VECTOR INIT_SCALE_FOR_TEST = { 3.0f,3.0f,3.0f };
}

StageObjectBase::StageObjectBase(void)
{
}

StageObjectBase::~StageObjectBase(void)
{
}

void StageObjectBase::Draw(void)
{
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
	ResourceManager& rsM = ResourceManager::GetInstance();
	modelId_ = rsM.Load(ResourceManager::SRC::STAGE_MDL).handleId_;

	//コライダ
	std::unique_ptr<Geometry> geo = std::make_unique<Model>(pos_, pos_, quaRot_, 20000.0f, modelId_);
	MakeCollider(std::move(geo), Collider::COL_TAG::STAGE, { Collider::COL_TAG::PLAYER,Collider::COL_TAG::ENEMY });

	pos_ = INIT_POS;

	material_ = std::make_unique<ModelMaterial>(L"UVScalingVS.cso", VS_BUFF_NUM, L"StdModelPS.cso", PS_BUFF_NUM);
	material_->AddConstBufVS(SCALING_UV);

	scl_ = INIT_SCALE_FOR_TEST;

	renderer_ = std::make_unique<ModelRenderer>();
}

void StageObjectBase::DoInit(void)
{
}

void StageObjectBase::DoUpdate(void)
{
}
