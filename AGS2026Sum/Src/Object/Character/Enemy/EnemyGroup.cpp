#include "../../../pch.h"
#include "../../Common/Collider.h"
#include "../../Common/Geometry/Sphere.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"

EnemyGroup::EnemyGroup(const int _num)
	: pos_(Utility::VECTOR_INIT)
	,initNum_(_num)
{
}

EnemyGroup::~EnemyGroup(void)
{
}

void EnemyGroup::Init(void)
{
	//座標の初期化
	pos_ = Utility::VECTOR_ZERO;

	//敵の生成
	CreateEnemy();
}

void EnemyGroup::Update(void)
{
	//ゴール地点に向かう
	MoveToGoal();

	for(auto& enemy : enemys_)
	{
		enemy->Update();
	}

	//敵の死亡時の処理
	DeleteEnemy();
}

void EnemyGroup::Draw(void)
{
	for(auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyGroup::Release(void)
{
	for (auto& enemy : enemys_)
	{
		enemy->Release();
	}
}

void EnemyGroup::CreateEnemy(void)
{
	//敵
	std::unique_ptr<EnemyBase> enemy;

	//生成
	for (int i = 0; i < initNum_;i++)
	{
		//ランダムな初期座標を生成
		VECTOR randPos;
		randPos.x = pos_.x + Utility::GetRandomValue(-LEAVE_GROUP_DIST, LEAVE_GROUP_DIST);
		randPos.y = 0.0f;
		randPos.z = pos_.z + Utility::GetRandomValue(-LEAVE_GROUP_DIST, LEAVE_GROUP_DIST);

		//敵の生成
		enemy = std::make_unique<EnemyBase>(randPos, movePow_);

		//読み込みと初期化
		enemy->Load();
		enemy->Init();

		//格納
		enemys_.push_back(std::move(enemy));
	}	
}

void EnemyGroup::DeleteEnemy(void)
{
	//そもそも敵がいないなら何もしない
	if (enemys_.empty()) return;

	//死亡した敵の削除
	std::erase_if(enemys_, [this](std::unique_ptr<EnemyBase>& _enemy) {return !_enemy->IsAlive(); });
}

void EnemyGroup::MoveToGoal(void)
{
	//移動量の設定
	movePow_ = Utility::GetMoveVec(pos_, groupGoalPos_, SPEED);

	//グループ座標の更新
	pos_ = VAdd(pos_, movePow_);
}
