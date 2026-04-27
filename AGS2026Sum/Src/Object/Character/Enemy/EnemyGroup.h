#pragma once
#include<vector>
#include<memory>

class EnemyBase;

class EnemyGroup
{
public:

	//敵がグループから離れられる距離
	static constexpr float LEAVE_GROUP_DIST = 200.0f;

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

	//グループ座標の取得
	const VECTOR& GetPos(void)const { return pos_; }

	//グループの目標座標の取得
	void SetGoalPos(const VECTOR& _goalPos){groupGoalPos_ = _goalPos;}

private:

	//当たり判定
	static constexpr float RADIUS = 30.0f;
	static constexpr float BROUD_RADIUS = RADIUS + 15.0f;
	static constexpr float SPEED = 5.0f;

	//全体関係
	VECTOR pos_;				//グループ座標
	VECTOR groupGoalPos_;		//グループの目標座標
	VECTOR movePow_;			//グループの移動量

	//敵情報
	int initNum_;										//初期人数
	std::vector<std::unique_ptr<EnemyBase>> enemys_;	//敵の情報

	//敵の生成
	void CreateEnemy(void);

	//死亡した敵の削除
	void DeleteEnemy(void);

	//ゴール地点に向かう移動量を設定
	void MoveToGoal(void);

	//グループから離れすぎた敵をグループに戻す
	void ReturnEnemyToGroup(void);
};

