#include "../../pch.h"
#include "../../Manager/Generic/ResourceManager.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include "StageManager.h"

namespace {
	const VECTOR INIT_POS = { 0.0f,0.0f,0.0f };
	const int VS_BUFF_NUM = 1;
	const int PS_BUFF_NUM = 0;

	const float SCALING_X = 2.0f;
	const float SCALING_Y = 2.0f;

	const FLOAT4 SCALING_UV = { SCALING_X,SCALING_Y,0.0f,0.0f };
}

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Draw(void)
{
	renderer_->Draw();
}

void StageManager::Release(void)
{
}

void StageManager::DoLoad(void)
{
}

void StageManager::DoInit(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	modelId_ = rsM.Load(ResourceManager::SRC::STAGE_MDL).handleId_;

	pos_ = INIT_POS;

	material_ = std::make_unique<ModelMaterial>(L"UVScalingVS.cso", VS_BUFF_NUM, L"StdModelPS.cso", PS_BUFF_NUM);
	material_->AddConstBufVS(SCALING_UV);

	renderer_ = std::make_unique<ModelRenderer>(modelId_, *material_);
}

void StageManager::DoUpdate(void)
{
}
