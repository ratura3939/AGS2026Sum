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

	//プレイヤーを狙う
	LookPlayer();

	//攻撃態勢に入る
	AttackReady();

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

void EnemyManager::LookPlayer(void)
{
	//グループが空なら処理しない
	if (enemyGroup_.empty())return;

	for (auto& group : enemyGroup_)
	{
		//プレイヤーから一定距離以上離れているグループは無視する
		if (Utility::Distance(group->GetPos(), playerPos_) > PLAYER_AIM_RADIUS)
		{
			//グループを待機状態にする
 			group->ChangeState(EnemyGroup::GROUP_STATE::STAY);
			continue;
		}

		//移動状態にする
		group->ChangeState(EnemyGroup::GROUP_STATE::MOVE);

		//グループの目標座標をプレイヤー座標に設定
		group->SetGoalPos(playerPos_);
	}
}

void EnemyManager::AttackReady(void)
{
	//グループが空なら処理しない
	if (enemyGroup_.empty())return;

	for (auto& group : enemyGroup_)
	{
		//プレイヤーから一定距離以上離れているグループは無視する
		if (Utility::Distance(group->GetPos(), playerPos_) > PLAYER_ATTACK_RADIUS)continue;

		//グループを攻撃態勢にする
		group->ChangeState(EnemyGroup::GROUP_STATE::ATTACK_READY);
	}
}
