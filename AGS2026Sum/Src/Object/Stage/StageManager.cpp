#include "../../pch.h"
#include "../../Manager/Generic/ResourceManager.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include"StageObjectBase.h"
#include"AutoDoor.h"
#include "StageManager.h"

namespace {
	const VECTOR INIT_POS_FIRST_STAGE = { 5000,0.0f,1000.0f };	//初期ステージ初期位置
	const VECTOR INIT_POS_BOSS_STAGE = { 5000.0f,0.0f,11000.0f };
	const int PS_BUFF_NUM = 0;

	const float SCALING_X = 1.0f;
	const float SCALING_Y = 1.0f;

	const FLOAT4 SCALING_UV = { SCALING_X,SCALING_Y,0.0f,0.0f };

	const VECTOR INIT_SCALE_FOR_TEST = { 3.0f,3.0f,3.0f };

	const VECTOR INIT_POS_DOOR = { 4700.0f,0.0f,6900.0f };	//ドアの初期位置}
}

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Load(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();

	//ステージオブジェクトの生成
	firstStage_ = std::make_unique<StageObjectBase>(resM.LoadModelDuplicate(ResourceManager::SRC::FIRST_STAGE_MDL), INIT_POS_FIRST_STAGE);
	firstStage_->Load();

	//ステージオブジェクトの生成
	bossStage_ = std::make_unique<StageObjectBase>(resM.LoadModelDuplicate(ResourceManager::SRC::BOSS_STAGE_MDL), INIT_POS_BOSS_STAGE);
	bossStage_->Load();

	//ドアの生成
	door_ = std::make_unique<AutoDoor>(INIT_POS_DOOR);
	door_->Load();
}

void StageManager::Init(void)
{
	firstStage_->Init();
	bossStage_->Init();
	door_->Init();
}

void StageManager::Update(void)
{
	firstStage_->Update();
	bossStage_->Update();
	door_->Update();
}

void StageManager::Draw(void)
{
	firstStage_->Draw();
	bossStage_->Draw();
	door_->Draw();
}

void StageManager::Release(void)
{
	firstStage_->Release();
	bossStage_->Release();
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

const VECTOR& StageManager::GetDoorPos(void) const
{
	return door_->GetPos();
}

const VECTOR& StageManager::GetGoalPosAtDoorOpen(void)const
{
	return door_->GetCameraGoalPosOfDoorEvent();
}
