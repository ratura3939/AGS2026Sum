#include "../../pch.h"
#include "MissionBase.h"
#include "MissionManager.h"

void MissionManager::Update(void)
{
	if (mission_)mission_->Update();
}

void MissionManager::Draw(void)
{
	if (mission_)mission_->Draw();
}

void MissionManager::SetMission(std::unique_ptr<MissionBase> _mission)
{
	mission_ = std::move(_mission);
}

void MissionManager::AddProgress(const ENEMY_TYPE& _enemy)
{
	mission_->AddProgress(_enemy);
}

const bool MissionManager::IsCompleted(void) const
{
	return mission_->IsCompleted();
}

MissionManager::MissionManager(void)
{
}

MissionManager::~MissionManager(void)
{
}