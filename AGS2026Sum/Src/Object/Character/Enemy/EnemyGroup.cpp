#include "../../../pch.h"
#include "../../Common/Collider.h"
#include "../../Common/Geometry/Sphere.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"

EnemyGroup::EnemyGroup(const int _num)
	: pos_(Utility::VECTOR_INIT)
	,initNum_(_num)
	, aliveNum_(0)
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
		enemy = std::make_unique<EnemyBase>(pos_, i);

		//読み込みと初期化
		enemy->Load();
		enemy->Init();

		//格納
		enemys_.push_back(std::move(enemy));

		//生存数のカウント
		aliveNum_++;
	}

	
}

void EnemyGroup::DeleteEnemy(void)
{
	//そもそも敵がいないなら何もしない
	if (aliveNum_ == 0) return;

	//死亡した敵の削除
	std::erase_if(enemys_, [this](const std::unique_ptr<EnemyBase>& enemy)
		{
			if (!enemy->IsAlive())
			{
				//死亡済み

				//解放
				enemy->Release();

				//生存数を減らす
				aliveNum_--;

				return true;
			}

			//生存中
			return false;
		});
}
