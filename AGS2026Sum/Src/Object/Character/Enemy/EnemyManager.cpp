#include"../../pch.h"
#include"../../Application.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/GameSystem/AttackManager.h"
#include"../../Scene/Main/Game.h"
#include"../../Utility/Utility.h"
#include"EnemyGroup.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(const VECTOR& _pPos)
	:playerPos_(_pPos)
{
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(void)
{
	//敵の生成(デバッグ)
	CreateEnemyGroup();
}

void EnemyManager::Update(void)
{
	//プレイヤーを狙う
	LookPlayer();

	//更新
	for (auto& group : enemyGroup_)
	{
		group->Update();
	}
}

void EnemyManager::Draw(void)
{
	//描画
	for (auto& group : enemyGroup_)
	{
		group->Draw();
	}
}

void EnemyManager::Release(void)
{
	//解放
	for (auto& group : enemyGroup_)
	{
		group->Release();
	}
}

void EnemyManager::CreateEnemyGroup(void)
{
	//グループ
	std::unique_ptr<EnemyGroup> group = std::make_unique<EnemyGroup>(CREATE_NUM);

	//初期化
	group->Init();

	//格納
	enemyGroup_.push_back(std::move(group));
}

void EnemyManager::LookPlayer(void)
{
	for (auto& group : enemyGroup_)
	{
		//プレイヤーから一定距離以上離れているグループは無視する
		if (Utility::Distance(group->GetPos(), playerPos_) > PLAYER_AIM_RADIUS)continue;

		//グループの目標座標をプレイヤー座標に設定
		group->SetGoalPos(playerPos_);
	}
}
