#pragma once
#include "../CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:

	//敵個々の状態
	enum class ENEMY_STATE
	{
		NONE = -1			//なし
		, STAY				//待機
		, MOVE				//移動
		, ATTACK_READY		//攻撃準備
		, ATTACK			//攻撃
	};

	//コンストラクタ
	EnemyBase(const VECTOR& _initPos, const VECTOR& _movePow);

	//デストラクタ
	~EnemyBase(void);

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//衝突後の処理
	void HitCollider(std::weak_ptr<Collider> _col)override;

	//状態遷移
	void ChangeState(const ENEMY_STATE _nextState);

private:

	//状態ごとの処理
	struct StateFunc
	{
		std::function<void(void)> enter = []() {};	//状態遷移時の処理
		std::function<void(void)> update = []() {};	//更新
		std::function<void(void)> exit = []() {};	//状態抜けの処理
	};

	//当たり判定
	static constexpr float RADIUS = 30.0f;
	static constexpr float BROUD_RADIUS = RADIUS + 15.0f;

	//攻撃距離
	static constexpr float ATTACK_DISTANCE = RADIUS + 15.0f;

	//初期座標
	const VECTOR initPos_;

	//グループの移動量
	const VECTOR& groupMovePow_;

	//状態
	ENEMY_STATE state_;
	std::unordered_map<ENEMY_STATE, StateFunc> stateFunc_;

	//読み込み
	void DoLoad(void)override;

	//初期化
	void DoInit(void)override;

	//更新
	void DoUpdate(void)override;

	//描画(デバッグ)
	void DrawDebug(void);

	//状態遷移時の処理
	void EnterStay(void);
	void EnterMove(void);
	void EnterAttackReady(void);
	void EnterAttack(void);

	//状態ごとの更新
	void UpdateStay(void);
	void UpdateMove(void);
	void UpdateAttackReady(void);
	void UpdateAttack(void);

	//状態抜けの処理
	void ExitStay(void);
	void ExitMove(void);
	void ExitAttackReady(void);
	void ExitAttack(void);

	//方向転換
	void ChangeDir(void);

	//移動処理
	void Move(void)override;

	//攻撃処理
	void Attack(void)override;
};