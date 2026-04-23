#pragma once
#include<vector>
#include<memory>

class EnemyBase;

class EnemyGroup
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_num">生成人数</param>
	EnemyGroup(const int _num);

	//デストラクタ
	~EnemyGroup(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Release(void);

private:

	//敵がグループから離れられる距離
	static constexpr float LEAVE_GROUP_DIST = 200.0f;

	//全体関係
	VECTOR pos_;										//グループ座標

	//敵情報
	int initNum_;										//初期人数
	int aliveNum_;										//生存人数
	std::vector<std::unique_ptr<EnemyBase>> enemys_;	//敵の情報

	//敵の生成
	void CreateEnemy(void);

	//死亡した敵の削除
	void DeleteEnemy(void);
};

