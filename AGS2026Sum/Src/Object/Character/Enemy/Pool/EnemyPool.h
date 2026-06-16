#pragma once
#include"../EnemyFactory.h"

class EnemyBase;

class EnemyPool
{
public:
	
	//コンストラクタ
	EnemyPool(void);
	
	//デストラクタ
	~EnemyPool(void);

	//初期化
	void Load(void);

	//解放
	void Release(void);

	/// <summary>
	/// 敵の生成
	/// </summary>
	/// <returns>参照用のポインタを返す</returns>
	EnemyBase* Spawn(const ENEMY_TYPE& _type);

	/// <summary>
	/// 敵の削除
	/// </summary>
	/// <param name="_enemy">削除する敵のポインタ</param>
	void Remove(EnemyBase* _enemy);

	//生存している敵の参照用リストの取得
	const std::vector<EnemyBase*>& GetActiveEnemys(void)const { return activeEnemys_; }

private:

	//敵ファクトリー
	EnemyFactory factory_;

	//敵
	std::vector<std::unique_ptr<EnemyBase>> allEnemys_;

	//稼働中の敵
	std::vector<EnemyBase*> activeEnemys_;

	//非稼働中の敵
	std::array<std::vector<EnemyBase*>,static_cast<int>(ENEMY_TYPE::MAX)> inactiveEnemys_;
};

