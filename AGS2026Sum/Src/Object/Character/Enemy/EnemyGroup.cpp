#include "../../../pch.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"

EnemyGroup::EnemyGroup(const int _num)
	: createNum_(_num)
{
}

EnemyGroup::~EnemyGroup(void)
{
}

void EnemyGroup::Init(void)
{
	//敵の生成
	CreateEnemy();
}

void EnemyGroup::Update(void)
{
	for(auto& enemy : enemys_)
	{
		enemy->Update();
	}
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
	for (int i = 0; i < createNum_;i++)
	{
		enemy = std::make_unique<EnemyBase>(i);

		//読み込みと初期化
		enemy->Load();
		enemy->Init();

		//格納
		enemys_.push_back(std::move(enemy));
	}
}
