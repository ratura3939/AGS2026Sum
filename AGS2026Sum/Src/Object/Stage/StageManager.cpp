#include "../../pch.h"
#include "../../Manager/Generic/ResourceManager.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include"AutoDoor.h"
#include "StageManager.h"

namespace {
	const VECTOR INIT_POS_FIRST_STAGE = { -2000.0f,0.0f,-1000.0f };	//初期ステージ初期位置
	const int VS_BUFF_NUM = 1;
	const int PS_BUFF_NUM = 0;

	const float SCALING_X = 1.0f;
	const float SCALING_Y = 1.0f;

	const FLOAT4 SCALING_UV = { SCALING_X,SCALING_Y,0.0f,0.0f };

	const VECTOR INIT_SCALE_FOR_TEST = { 3.0f,3.0f,3.0f };

	const VECTOR INIT_POS_DOOR = { 0.0f,0.0f,500.0f };	//ドアの初期位置}
}

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Draw(void)
{
	renderer_->Draw(modelId_, *material_);
	door_->Draw();
}

void StageManager::Release(void)
{
}

void StageManager::OpenDoor(void)
{
	door_->OpenDoor();
}

void StageManager::CloseDoor(void)
{
	door_->CloseDoor();
}

void StageManager::DoLoad(void)
{
	quaRotLocal_ = Quaternion::Euler(0.0f, 0.0f, 0.0f);
}

void StageManager::DoInit(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	modelId_ = rsM.Load(ResourceManager::SRC::STAGE_MDL).handleId_;

	pos_ = INIT_POS_FIRST_STAGE;

	material_ = std::make_unique<ModelMaterial>(L"UVScalingVS.cso", VS_BUFF_NUM, L"StdModelPS.cso", PS_BUFF_NUM);
	material_->AddConstBufVS(SCALING_UV);

	scl_ = INIT_SCALE_FOR_TEST;

	renderer_ = std::make_unique<ModelRenderer>();

	//ドアの生成
	door_ = std::make_unique<AutoDoor>(INIT_POS_DOOR);
	door_->Init();
}

void StageManager::DoUpdate(void)
{
	door_->Update();
}
