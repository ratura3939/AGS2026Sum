#include "../../../pch.h"
#include "EnemyBase.h"
#include "EnemyPool.h"

EnemyPool::EnemyPool(void)
{
}

EnemyPool::~EnemyPool(void)
{
}

void EnemyPool::Init(void)
{
	//敵の生成や初期化はSpawnで行うのでここでは行わない
}

void EnemyPool::Update(void)
{
	//生存している敵の更新
	for (auto& enemy : activeEnemys_)
	{
		enemy->Update();
	}
}

void EnemyPool::Draw(void)
{
	//生存している敵の描画
	for (auto& enemy : activeEnemys_)
	{
		enemy->Draw();
	}
}

void EnemyPool::Release(void)
{
	//参照ポインタの削除
	activeEnemys_.clear();
	inactiveEnemys_.clear();

	//敵の削除(持ち主自体が消えるのでそのまま消去)
	allEnemys_.clear();
}

EnemyBase* EnemyPool::Spawn(void)
{
	//生成済みの敵でもう死んでいるものがいるならそれを再利用する
	if (!inactiveEnemys_.empty())
	{
		//再利用する敵のポインタ
		EnemyBase* enemy = inactiveEnemys_.back();

		//再初期化(Loadは済んでいるので行わない)
		enemy->Init();

		//生存リストの末尾に追加するのでその添え字を設定
		enemy->SetActiveIndex(activeEnemys_.size());

		//再利用する敵を生存リストの末尾に追加
		activeEnemys_.push_back(enemy);

		//非稼働中の敵のリストから再利用する敵を削除
		inactiveEnemys_.pop_back();

		//再利用した敵のポインタを返す
		return enemy;
	}

	//再利用できる敵がいないなら新たに生成する
	std::unique_ptr<EnemyBase> enemy = std::make_unique<EnemyBase>();

	//読み込みと初期化
	enemy->Load();
	enemy->Init();

	//生成した敵の添え字を設定
	enemy->SetActiveIndex(activeEnemys_.size());

	//生成した敵の参照用生ポインタ
	EnemyBase* enemyPtr = enemy.get();

	//全体の敵リストに追加
	allEnemys_.push_back(std::move(enemy));

	//生存リストの末尾に追加
	activeEnemys_.push_back(enemyPtr);

	//生成した敵のポインタを返す
	return enemyPtr;
}

void EnemyPool::Remove(EnemyBase* _enemy)
{
	//死亡した敵の添え字
	int index = _enemy->GetActiveIndex();

	//末尾の敵が死亡したならスワップを行わない
	if (index != activeEnemys_.size() - 1)
	{
		//生存リストの末尾の敵
		EnemyBase* backEnemy = activeEnemys_.back();

		//死亡した敵と生存リストの末尾の敵を入れ替える
		std::swap(activeEnemys_[index], activeEnemys_.back());

		//入れ替えた生存中の敵の添え字を更新する
		backEnemy->SetActiveIndex(index);
	}

	//死亡した敵を非稼働中の敵のリストに追加
	inactiveEnemys_.push_back(_enemy);

	//死亡した敵を稼働中の敵のリストから削除
	activeEnemys_.pop_back();
}