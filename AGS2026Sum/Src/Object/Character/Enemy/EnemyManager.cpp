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
	//グループの削除処理
	DeleteEnemyGroup();

	//距離ごとの行動決め
	DistanceAction();

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

void EnemyManager::DeleteEnemyGroup(void)
{
	//グループが空なら処理しない
	if (enemyGroup_.empty())return;

	//グループの削除処理
	std::erase_if(enemyGroup_, [](const std::unique_ptr<EnemyGroup>& _group) {return _group->IsEmpty();});
}

void EnemyManager::DistanceAction(void)
{
	//グループが空なら処理しない
	if (enemyGroup_.empty())return;

	for (auto& group : enemyGroup_)
	{
		//グループが空なら処理しない
		if (group->IsEmpty())continue;

		//プレイヤーからの距離を取得
		float dist = Utility::Distance(group->GetPos(), playerPos_);

		//プレイヤーから一定距離以上離れているグループは無視する
		if (dist < PLAYER_ATTACK_RADIUS)
		{
			//グループを攻撃準備状態にする
			group->ChangeState(EnemyGroup::GROUP_STATE::ATTACK_READY);
			
			//グループの目標座標をプレイヤー座標に設定
			group->SetGoalPos(playerPos_);
		}
		if (dist < PLAYER_AIM_RADIUS)
		{
			//グループを移動状態にする
			group->ChangeState(EnemyGroup::GROUP_STATE::MOVE);

			//グループの目標座標をプレイヤー座標に設定
			group->SetGoalPos(playerPos_);
		}
		else
		{
			//グループを待機状態にする
			group->ChangeState(EnemyGroup::GROUP_STATE::STAY);
		}
	}
}