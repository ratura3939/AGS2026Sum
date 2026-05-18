#include"../../pch.h"
#include"../../Application.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/GameSystem/AttackManager.h"
#include"../../Scene/Main/Game.h"
#include"../../Utility/Utility.h"
#include"EnemyGroup.h"
#include"EnemyPool.h"
#include"EnemyBase.h"
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
	//敵のプールを生成
	enemyPool_ = std::make_unique<EnemyPool>();

	//敵の生成(デバッグ)
	CreateEnemyGroup();
}

void EnemyManager::Update(void)
{
	//距離ごとの行動決め
	DistanceAction();

	//更新
	for (auto& group : enemyGroup_)
	{
		group->Update();
	}
	enemyPool_->Update();

	//グループの削除処理
	DeleteEnemyGroup();

	//グループに所属していない敵を別グループに再所属させる
	ReJoinGroups();

	//敵の削除処理
	DeleteEnemy();
}

void EnemyManager::Draw(void)
{
	//描画
	for (auto& group : enemyGroup_)
	{
		group->Draw();
	}
	enemyPool_->Draw();
}

void EnemyManager::Release(void)
{
	//解放
	for (auto& group : enemyGroup_)
	{
		group->Release();
	}
	enemyPool_->Release();
}

void EnemyManager::CreateEnemyGroup(void)
{
	//グループ
	std::unique_ptr<EnemyGroup> group = std::make_unique<EnemyGroup>();

	//初期化
	group->Init();

	//敵の参照用ポインタ
	EnemyBase* enemy = nullptr;

	//指定分、敵を生成する
	for (int i = 0; i < CREATE_NUM; i++)
	{
		//生成
		enemy = enemyPool_->Spawn();

		//グループに設定
		Grouping(group.get(), enemy);
		enemy->InitWithGroup();
	}

	//格納
	enemyGroup_.push_back(std::move(group));
}

void EnemyManager::Grouping(EnemyGroup* _group, EnemyBase* _enemy)
{
	//グループに所属させる
	_group->AddEnemy(_enemy);
	_enemy->SetGroup(_group);
}

void EnemyManager::DeleteEnemy(void)
{
	//そもそも敵がいないなら何もしない
	if (!enemyPool_) return;

	//死亡した敵　または　グループに所属していない敵の削除
	for (auto& activeEnemy : enemyPool_->GetActiveEnemys())
	{
		if (!activeEnemy->IsAlive() || !activeEnemy->IsInGroup())
		{
			enemyPool_->Remove(activeEnemy);
		}
	}
}

void EnemyManager::DeleteEnemyGroup(void)
{
	//グループが空なら処理しない
	if (enemyGroup_.empty())return;

	//グループの削除処理
	std::erase_if(enemyGroup_, [](const std::unique_ptr<EnemyGroup>& _group) {return _group->IsEmpty();});
}

void EnemyManager::ReJoinGroups(void)
{
	//グループ　または　敵が空なら処理しない
	if (enemyGroup_.empty() || !enemyPool_)return;

	//敵グループの末尾
	EnemyGroup* enemyGroupBack = enemyGroup_.back().get();

	//グループに所属していない敵を別グループに再所属させる
	for (auto& enemy : enemyPool_->GetActiveEnemys())
	{
		//所属しているか
		if (!enemy->IsInGroup())
		{
			//再所属
			Grouping(enemyGroupBack, enemy);
		}
	}
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
		else if (dist < PLAYER_AIM_RADIUS)
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