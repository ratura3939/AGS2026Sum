#pragma once
#include<vector>
#include"../EnemyGroup.h"

class EnemyGroupPool
{
public:

	//コンストラクタ
	EnemyGroupPool(void);

	//デストラクタ
	~EnemyGroupPool(void);

	//初期化
	void Load(void);

	//解放
	void Release(void);

	/// <summary>
	/// 敵グループの生成
	/// </summary>
	/// <param name="_initPos">グループの初期座標</param>
	/// <returns>参照用のポインタを返す</returns>
	EnemyGroup* Spawn(const VECTOR& _initPos);

	/// <summary>
	/// 敵グループの削除
	/// </summary>
	/// <param name="_enemyGroup">削除する敵グループのポインタ</param>
	void Remove(EnemyGroup* _enemyGroup);

	//生存している敵グループの参照用リストの取得
	const std::vector<EnemyGroup*>& GetActiveEnemyGroups(void)const { return activeEnemyGroups_; }

	//生存敵グループの末尾を取得
	EnemyGroup* GetActiveEnemyGroupBack(void)const;

private:

	//敵グループ
	std::vector<std::unique_ptr<EnemyGroup>> allEnemyGroups_;

	//稼働中の敵グループ
	std::vector<EnemyGroup*> activeEnemyGroups_;

	//非稼働中の敵グループ
	std::vector<EnemyGroup*> inactiveEnemyGroups_;
};

