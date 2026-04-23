#pragma once
#include<memory>
#include<vector>
#include<DxLib.h>
#include"../../Common/Quaternion.h"

class EnemyGroup;
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

	//敵の生成数
	static constexpr int CREATE_NUM = 7;

	//敵グループ
	std::vector<std::unique_ptr<EnemyGroup>> enemyGroup_;	

	//プレイヤー座標
	const VECTOR& playerPos_;		
};

