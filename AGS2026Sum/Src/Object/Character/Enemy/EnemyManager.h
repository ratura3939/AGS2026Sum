#pragma once
#include<memory>
#include<vector>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyBase;
class EnemyPool;
class EnemyGroup;
class EnemyGroupPool;
class Game;
class AttackManager;

class EnemyManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_pPos">プレイヤー座標</param>
	EnemyManager(const VECTOR& _pPos);
	
	//デストラクタ
	~EnemyManager(void);

	//初期化
	void Init(void);
	
	//更新
	void Update(void);
		
	//描画
	void Draw(void);
	
	//解放
	void Release(void);

	//敵グループの生成
	void CreateEnemyGroup(const int _createNum);

	//生存中の敵の数を取得
	const int GetActiveEnemyNum(void)const;

private:

	//プレイヤーを狙うときの距離半径
	static constexpr float PLAYER_AIM_RADIUS = 1000.0f;

	//プレイヤーを攻撃態勢に入る距離半径
	static constexpr float PLAYER_ATTACK_RADIUS = 500.0f;

	//チャンク管理用の初期確保数
	static constexpr int INIT_CHUNK_GROUP_NUM = 100;

	//グループを維持できる最小の敵の数
	static constexpr int MIN_ENEMY_NUM = 3;

	//敵の生成数
	static constexpr int CREATE_NUM = 7;

	//チャンク範囲
	static constexpr int CHUNK_RANGE = 1;

	//敵グループ
	std::unique_ptr<EnemyGroupPool> enemyGroup_;

	//チャンク内の敵グループ(参照用)
	std::vector<EnemyGroup*> chunkGroups_;

	//敵情報
	std::unique_ptr<EnemyPool> enemyPool_;

	//プレイヤー座標
	const VECTOR& playerPos_;

	//グループと敵の関連付け
	void Grouping(EnemyGroup* _group, EnemyBase* _enemy);

	//敵の削除処理
	void DeleteEnemy(void);

	//グループの削除処理
	void DeleteEnemyGroup(void);

	//グループに所属していない敵を別グループに再所属させる
	void ReJoinGroups(void);

	//距離ごとの行動決め
	void DecideOrderByDistance(void);
};