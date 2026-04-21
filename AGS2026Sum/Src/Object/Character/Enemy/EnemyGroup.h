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

	//生成人数
	int createNum_;

	//敵
	std::vector<std::unique_ptr<EnemyBase>> enemys_;

	//敵の生成
	void CreateEnemy(void);
};

