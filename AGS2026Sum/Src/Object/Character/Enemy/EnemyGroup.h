#pragma once
#include<DxLib.h>
#include<vector>
#include<memory>
#include<functional>
#include "Info/EnemyDefine.h"

class EnemyBase;

class EnemyGroup
{
public:

	//コンストラクタ
	EnemyGroup(void);

	//デストラクタ
	~EnemyGroup(void);

	//初期化
	void Init(const VECTOR& _initPos);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Release(void);

	//敵がいなくなったか
	bool IsEmpty(void)const { return enemys_.empty(); }

	//稼働中添え字の取得
	const int GetActiveIndex(void)const { return activeIndex_; }

	//稼働中添え字の設定
	void SetActiveIndex(const int _index) { activeIndex_ = _index; }

	//チャンク管理用の添え字の取得
	const int GetChunkIndex(void)const { return chunkIndex_; }

	//チャンク管理用の添え字の設定
	void SetChunkIndex(const int _index) { chunkIndex_ = _index; }

	//リーダー座標の取得
	const VECTOR& GetLeaderPos(void)const;

	//リーダーの設定
	void SetLeader(const EnemyBase* _leader);

	//初期座標の取得
	const VECTOR& GetInitPos(void)const { return initPos_; }

	//グループの移動量の取得
	const VECTOR& GetMovePow(void)const { return movePow_; }

	//グループの目標座標の取得
	const VECTOR& GetGoalPos(void)const { return groupGoalPos_; }

	//グループの目標座標の取得
	void SetGoalPos(const VECTOR& _goalPos){groupGoalPos_ = _goalPos;}

	//状態取得
	const GROUP_ORDER GetOrder(void)const { return order_; }

	//状態遷移
	void ChangeOrder(const GROUP_ORDER _nextOrder);

	//位置リセット
	void ResetPos(void);

	//敵の追加
	void AddEnemy(EnemyBase* _enemy) { enemys_.push_back(_enemy); }

	//敵の数の取得
	const int GetEnemyCount(void)const { return static_cast<int>(enemys_.size()); }

	//生存状態の取得
	const bool IsActive(void)const { return isActive_; }

	//死亡
	void Kill(void) { isActive_ = false; }

private:

	//攻撃を開始する距離
	static constexpr float ATTACK_DISTANCE = 150.0f;

	//速度
	static constexpr float SPEED = 5.0f;

	//命令の関数ポインタ
	using Func = void(EnemyGroup::*)(void);

	//命令ごとの処理
	struct OrderFunc
	{
		Func enter = nullptr;	//命令遷移時の処理
		Func update = nullptr;	//更新
		Func exit = nullptr;	//命令抜けの処理
	};

	//行動切り替えの間隔
	static constexpr float ACTION_INTERVAL = 2.0f;

	//稼働中添え字
	int activeIndex_;

	//チャンク管理用の添え字
	int chunkIndex_;

	//全体関係
	const EnemyBase* leader_;	//リーダー
	VECTOR initPos_;			//初期座標
	VECTOR groupGoalPos_;		//グループの目標座標
	VECTOR movePow_;			//グループの移動量

	//行動関係
	float actionCnt_;		//行動切り替えのカウント

	//生存状態
	bool isActive_;

	//命令
	GROUP_ORDER order_;										//グループの命令
	std::unordered_map<GROUP_ORDER, OrderFunc> orderFunc_;	//命令ごとの処理

	//敵情報
	std::vector<EnemyBase*> enemys_;	//敵の情報(Managerからの参照用)

	//死亡した敵の削除
	void DeleteEnemy(void);

	//リーダーの再設定
	void ResetLeader(void);

	//ゴール地点に向かう移動量を設定
	void MoveToGoal(void);

	//グループ移動
	void GroupMove(void);

	//状態遷移時の処理
	void EnterStay(void);			//待機
	void EnterMove(void);			//移動
	void EnterAlert(void);			//警戒

	//更新処理
	void UpdateStay(void);			//待機
	void UpdateMove(void);			//移動
	void UpdateAlert(void);			//警戒

	//状態抜けの処理
	void ExitStay(void);			//待機
	void ExitMove(void);			//移動
	void ExitAlert(void);			//警戒
};

