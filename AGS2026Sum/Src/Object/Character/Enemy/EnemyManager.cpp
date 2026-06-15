#include"../../pch.h"
#include"../../Utility/Utility.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include"../../../Manager/GameSystem/ChunkManager.h"
#include"EnemyParameter.h"
#include"EnemyDefine.h"
#include"EnemyGroup.h"
#include"EnemyGroupPool.h"
#include"EnemyPool.h"
#include"EnemyBase.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(const VECTOR& _pPos)
	:playerPos_(_pPos)
{
	//チャンク内の敵の管理用のリストにとりあえず初期確保数分の容量を確保しておく(確保・削除を減らすため)
	chunkGroups_.reserve(INIT_CHUNK_GROUP_NUM);
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Load(void)
{
	//敵グループのプールを生成
	enemyGroupPool_ = std::make_unique<EnemyGroupPool>();
	enemyGroupPool_->Load();

	//敵のプールを生成
	enemyPool_ = std::make_unique<EnemyPool>();
	enemyPool_->Load();
}

void EnemyManager::Init(void)
{
	const int ENEMY_GROUP_NUM = 0;
	const int MIDDLE_BOSS_GROUP_NUM = 1;

	//敵の生成(デバッグ)
	for (int i = 0; i < ENEMY_GROUP_NUM;i++)
	{
		CreateEnemyGroup(CREATE_NUM);
	}
	for (int i = 0; i < MIDDLE_BOSS_GROUP_NUM;i++)
	{
		CreateMiddleBossGroup(CREATE_NUM);
	}
}

void EnemyManager::Update(void)
{
	//インスタンス
	ChunkManager& chunkMng = ChunkManager::GetInstance();

	//距離ごとの命令決め
	DecideOrderByDistance();

	//チャンク管理リストを空にする
	chunkGroups_.clear();

	//チャンク管理用のリストを更新
	chunkMng.GetEnemyGroupsInRangeChunk(chunkGroups_, playerPos_, CHUNK_RANGE);

	//チャンク内のみ更新
	for (auto& group : chunkGroups_)
	{
		//更新
		group->Update();

		//チャンクの移動確認
		chunkMng.MoveEnemyGroup(group);
	}

	//グループの削除処理
	DeleteEnemyGroup();

	//グループに所属していない敵を別グループに再所属させる
	ReJoinGroups();

	//敵の削除処理
	DeleteEnemy();
}

void EnemyManager::Draw(void)
{
	//チャンク内のみ描画
	for (auto& group : chunkGroups_)
	{
		group->Draw();
	}
}

void EnemyManager::Release(void)
{
	//解放
	enemyGroupPool_->Release();
	enemyGroupPool_.reset();
	enemyPool_->Release();
	enemyPool_.reset();
}

void EnemyManager::CreateEnemyGroup(const int _createNum)
{
	//グループ
	EnemyGroup* group = enemyGroupPool_->Spawn();

	//初期化
	group->Init();

	//グループの初期座標(デバッグ)
	static VECTOR pos = { 0.0f, 0.0f, 0.0f };
	//group->SetPos(pos);
	//pos = VAdd(pos, { 1000.0f, 0.0f, 1000.0f });

	//グループのチャンク管理用の添え字を設定
	ChunkManager::GetInstance().AddEnemyGroup(group);

	//敵の参照用ポインタ
	EnemyBase* enemy = nullptr;

	//指定分、敵を生成する
	for (int i = 0; i < _createNum; i++)
	{
		//生成
		enemy = enemyPool_->Spawn(ENEMY_TYPE::NORMAL);

		//グループに設定
		Grouping(group, enemy);
		enemy->InitWithGroup();
	}
}

void EnemyManager::CreateMiddleBossGroup(const int _createNum)
{
	//グループ
	EnemyGroup* group = enemyGroupPool_->Spawn();

	//初期化
	group->Init();

	//グループの初期座標(デバッグ)
	static VECTOR pos = { 0.0f, 0.0f, 0.0f };
	//group->SetPos(pos);
	//pos = VAdd(pos, { 1000.0f, 0.0f, 1000.0f });

	//グループのチャンク管理用の添え字を設定
	ChunkManager::GetInstance().AddEnemyGroup(group);

	//敵の参照用ポインタ
	EnemyBase* enemy = nullptr;

	//中ボス
	enemy = enemyPool_->Spawn(ENEMY_TYPE::MIDDLE_BOSS);

	//指定分、敵を生成する
	for (int i = 0; i < _createNum; i++)
	{
		//生成
		enemy = enemyPool_->Spawn(ENEMY_TYPE::NORMAL);

		//グループに設定
		Grouping(group, enemy);
		enemy->InitWithGroup();
	}
}

const int EnemyManager::GetActiveEnemyNum(void) const
{
	//プールがないならそもそも0
	if (!enemyPool_) return 0;

	return static_cast<int>(enemyPool_->GetActiveEnemys().size());
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

	//削除する敵のリスト
	std::vector<EnemyBase*> removeEnemys;

	//死亡した敵　または　グループに所属していない敵の削除
	for (auto& activeEnemy : enemyPool_->GetActiveEnemys())
	{
		if (!activeEnemy->IsAlive() || !activeEnemy->IsInGroup())
		{
			//削除する敵のリストに追加
			removeEnemys.push_back(activeEnemy);
		}
	}

	//削除する敵のリストにいる敵を削除
	for (auto& removeEnemy : removeEnemys)
	{
		enemyPool_->Remove(removeEnemy);
	}
}

void EnemyManager::DeleteEnemyGroup(void)
{
	//グループが空なら処理しない
	if (!enemyGroupPool_)return;

	//削除するグループのリスト
	std::vector<EnemyGroup*> removeGroups;

	//活動中のグループの数
	int activeGroupNum = static_cast<int>(enemyGroupPool_->GetActiveEnemyGroups().size());

	//グループの削除処理
	for (auto& group : enemyGroupPool_->GetActiveEnemyGroups())
	{
		//グループに所属している敵の数が一定数以下　または　グループが空なら削除
		if (group->IsEmpty() || (group->GetEnemyCount() < MIN_ENEMY_NUM) && activeGroupNum > 1)
		{
			//削除するグループのリストに追加
			removeGroups.push_back(group);
		}
	}

	//削除するグループのリストにいるグループを削除
	for (auto& removeGroup : removeGroups)
	{
		//チャンク管理から削除
		ChunkManager::GetInstance().RemoveEnemyGroup(removeGroup);

		//グループに所属している敵をグループから抜けさせる
		enemyGroupPool_->Remove(removeGroup);
	}
}

void EnemyManager::ReJoinGroups(void)
{
	//グループ　または　敵が空なら処理しない
	if (!enemyGroupPool_ || !enemyPool_)return;

	//敵グループの末尾
	EnemyGroup* enemyGroupBack = enemyGroupPool_->GetActiveEnemyGroupBack();

	//グループの末尾が空なら処理しない
	if (!enemyGroupBack)return;

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

void EnemyManager::DecideOrderByDistance(void)
{
	//グループが空なら処理しない
	if (!enemyGroupPool_)return;

	for (auto& group : enemyGroupPool_->GetActiveEnemyGroups())
	{
		//グループが空なら処理しない
		if (group->IsEmpty())continue;

		//プレイヤーからの距離を取得
		float sqrDist = Utility::SqrMagnitude(group->GetPos(), playerPos_);

		//プレイヤーから一定距離以上離れているグループは無視する
		if (sqrDist < PLAYER_ATTACK_RADIUS * PLAYER_ATTACK_RADIUS)
		{
			//グループを攻撃準備状態にする
			group->ChangeOrder(GROUP_ORDER::ALERT);
			
			//グループの目標座標をプレイヤー座標に設定
			group->SetGoalPos(playerPos_);
		}
		else if (sqrDist < PLAYER_AIM_RADIUS * PLAYER_AIM_RADIUS)
		{
			//グループを移動状態にする
			group->ChangeOrder(GROUP_ORDER::MOVE);

			//グループの目標座標をプレイヤー座標に設定
			group->SetGoalPos(playerPos_);
		}
		else
		{
			//グループを待機状態にする
			group->ChangeOrder(GROUP_ORDER::STAY);
		}
	}
}