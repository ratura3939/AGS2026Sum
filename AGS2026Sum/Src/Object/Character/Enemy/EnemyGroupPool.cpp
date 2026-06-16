#include "../../../pch.h"
#include "EnemyGroupPool.h"

EnemyGroupPool::EnemyGroupPool(void)
{
}

EnemyGroupPool::~EnemyGroupPool(void)
{
}

void EnemyGroupPool::Load(void)
{
	//敵グループの生成や初期化はSpawnで行うのでここでは行わない
}

void EnemyGroupPool::Release(void)
{
	//参照ポインタの削除
	activeEnemyGroups_.clear();
	inactiveEnemyGroups_.clear();

	//敵グループの削除(持ち主自体が消えるのでそのまま消去)
	allEnemyGroups_.clear();
}

EnemyGroup* EnemyGroupPool::Spawn(void)
{
	//生成済みの敵グループでもう死んでいるものがいるならそれを再利用する
	if(!inactiveEnemyGroups_.empty())
	{
		//再利用する敵グループのポインタ
		EnemyGroup* enemyGroup = inactiveEnemyGroups_.back();

		//再初期化
		enemyGroup->Init();

		//生存リストの末尾に追加するのでその添え字を設定
		enemyGroup->SetActiveIndex(activeEnemyGroups_.size());

		//再利用する敵グループを生存リストの末尾に追加
		activeEnemyGroups_.push_back(enemyGroup);

		//非稼働中の敵グループのリストから再利用する敵グループを削除
		inactiveEnemyGroups_.pop_back();

		//再利用した敵グループのポインタを返す
		return enemyGroup;
	}

	//再利用できる敵グループがいないなら新たに生成する
	std::unique_ptr<EnemyGroup> enemyGroup = std::make_unique<EnemyGroup>();

	//初期化
	enemyGroup->Init();

	//生成した敵グループの添え字を設定
	enemyGroup->SetActiveIndex(activeEnemyGroups_.size());

	//生成したグループの参照用ポインタ
	EnemyGroup* enemyGroupPtr = enemyGroup.get();

	//生成した敵グループを全体のリストに追加
	allEnemyGroups_.push_back(std::move(enemyGroup));

	//生成した敵グループを生存リストの末尾に追加
	activeEnemyGroups_.push_back(enemyGroupPtr);

	//生成した敵グループのポインタを返す
	return enemyGroupPtr;
}

void EnemyGroupPool::Remove(EnemyGroup* _enemyGroup)
{
	//削除する敵グループの添え字
	int index = _enemyGroup->GetActiveIndex();

	//末尾の敵グループが死亡したなら入れ替えを行わない
	if (index != activeEnemyGroups_.size() - 1)
	{
		//生成リストの末尾
		EnemyGroup* enemyGroupBack = activeEnemyGroups_.back();

		//死亡した敵グループと生成リストの末尾の敵グループを入れ替える
		std::swap(activeEnemyGroups_[index], activeEnemyGroups_.back());

		//添え字の更新
		enemyGroupBack->SetActiveIndex(index);
	}

	//死亡判定
	_enemyGroup->Kill();

	//非稼働中の敵グループのリストに追加
	inactiveEnemyGroups_.push_back(_enemyGroup);

	//生存リストの末尾から削除
	activeEnemyGroups_.pop_back();
}

EnemyGroup* EnemyGroupPool::GetActiveEnemyGroupBack(void) const
{
	//敵がいないなら
	if (activeEnemyGroups_.size() == 0)return nullptr;

	//末尾を返す
	return activeEnemyGroups_.back(); 
}
