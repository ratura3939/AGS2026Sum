#pragma once
#include<vector>
#include<memory>
#include<functional>

class EnemyBase;

class EnemyGroup
{
public:

	//グループごとの状態
	enum class GROUP_STATE
	{
		NONE = -1			//なし
		, STAY				//待機
		, MOVE				//移動
		, ATTACK_READY		//攻撃準備
	};

	//敵がグループから離れられる距離
	static constexpr float LEAVE_GROUP_DIST = 1000.0f;

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

	//敵がいなくなったか
	bool IsEmpty(void)const { return enemys_.empty(); }

	//グループ座標の取得
	const VECTOR& GetPos(void)const { return pos_; }

	//グループの目標座標の取得
	void SetGoalPos(const VECTOR& _goalPos){groupGoalPos_ = _goalPos;}

	//状態遷移
	void ChangeState(const GROUP_STATE _nextState);

private:

	//攻撃を開始する距離
	static constexpr float ATTACK_DISTANCE = 150.0f;

	//状態ごとの処理
	struct StateFunc
	{
		std::function<void(void)> enter = [](){};	//状態遷移時の処理
		std::function<void(void)> update = [](){};	//更新
		std::function<void(void)> exit = [](){};	//状態抜けの処理
	};

	//移動速度
	static constexpr float SPEED = 5.0f;

	//行動切り替えの間隔
	static constexpr float ACTION_INTERVAL = 2.0f;

	//全体関係
	VECTOR pos_;			//グループ座標
	VECTOR groupGoalPos_;	//グループの目標座標
	VECTOR movePow_;		//グループの移動量

	//行動関係
	float actionCnt_;		//行動切り替えのカウント

	//状態
	GROUP_STATE state_;										//グループの状態
	std::unordered_map<GROUP_STATE, StateFunc> stateFunc_;	//状態ごとの処理

	//敵情報
	int initNum_;										//初期人数
	std::vector<std::unique_ptr<EnemyBase>> enemys_;	//敵の情報

	//敵の生成
	void CreateEnemy(void);

	//死亡した敵の削除
	void DeleteEnemy(void);

	//ゴール地点に向かう移動量を設定
	void MoveToGoal(void);

	//グループ移動
	void GroupMove(void);

	//状態遷移時の処理
	void EnterStay(void);			//待機
	void EnterMove(void);			//移動
	void EnterAttackReady(void);	//攻撃準備

	//更新処理
	void UpdateStay(void);			//待機
	void UpdateMove(void);			//移動
	void UpdateAttackReady(void);	//攻撃準備

	//状態抜けの処理
	void ExitStay(void);			//待機
	void ExitMove(void);			//移動
	void ExitAttackReady(void);		//攻撃準備
};

