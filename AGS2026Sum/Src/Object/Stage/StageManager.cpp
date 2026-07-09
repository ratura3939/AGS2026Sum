#include "../../pch.h"
#include "../../Manager/Generic/ResourceManager.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include"StageObjectBase.h"
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

void StageManager::Load(void)
{
	//ステージオブジェクトの生成
	object_ = std::make_unique<StageObjectBase>();
	object_->Load();
	object_->Init();

	//ドアの生成
	door_ = std::make_unique<AutoDoor>(INIT_POS_DOOR);
	door_->Init();
}

void StageManager::Init(void)
{
	object_->Init();
	door_->Init();
}

void StageManager::Update(void)
{
	object_->Update();
	door_->Update();
}

void StageManager::Draw(void)
{
	object_->Draw();
	door_->Draw();
}

void StageManager::Release(void)
{
	object_->Release();
	door_->Release();
}

void StageManager::OpenDoor(void)
{
	door_->OpenDoor();
}

void StageManager::CloseDoor(void)
{
	door_->CloseDoor();
}