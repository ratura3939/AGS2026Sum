#pragma once
#include<memory>
#include<vector>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyGroup;
class EnemyBase;
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
	void CreateEnemyGroup(void);

private:

	//敵がグループから離れられる距離
	static constexpr float LEAVE_GROUP_DIST = 1000.0f;

	//プレイヤーを狙うときの距離半径
	static constexpr float PLAYER_AIM_RADIUS = 1000.0f;

	//プレイヤーを攻撃態勢に入る距離半径
	static constexpr float PLAYER_ATTACK_RADIUS = 500.0f;

	//敵の生成数
	static constexpr int CREATE_NUM = 7;

	//敵グループ
	std::vector<std::unique_ptr<EnemyGroup>> enemyGroup_;	

	//敵の情報
	std::vector<std::unique_ptr<EnemyBase>> enemys_;	

	//プレイヤー座標
	const VECTOR& playerPos_;

	//敵の削除処理
	void DeleteEnemy(void);

	//グループの削除処理
	void DeleteEnemyGroup(void);

	//グループに所属していない敵を別グループに再所属させる
	void ReJoinGroups(void);

	//距離ごとの行動決め
	void DistanceAction(void);
};

