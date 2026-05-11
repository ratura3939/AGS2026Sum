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
		, RETURN			//グループに戻る
		, MAX
	};

	//コンストラクタ
	EnemyBase(const VECTOR& _initPos);

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

	//移動量の設定
	void SetMovePow(const VECTOR& _movePow) { movePow_ = _movePow; }

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
	static constexpr float ATTACK_RADIUS = RADIUS + 15.0f;
	static constexpr float ATTACK_BROUD_RADIUS = ATTACK_RADIUS + 15.0f;
	static constexpr VECTOR ATTACK_LOCAL_POS = { 0.0f, 0.0f, 30.0f };

	//アニメーション
	static constexpr float RUN_SPEED = 2.0f;			//走り速度

	//座標
	const VECTOR initPos_;	//初期座標
	VECTOR attackPos_;		//攻撃目標座標

	//個人の移動量
	VECTOR movePow_;

	//状態
	ENEMY_STATE state_;
	std::unordered_map<ENEMY_STATE, StateFunc> stateFunc_;

	//読み込み
	void DoLoad(void)override;

	//初期化
	void DoInit(void)override;

	//更新
	void DoUpdate(void)override;

	//アニメーションの初期化
	void InitAnim(void)override;

	//描画(デバッグ)
	void DrawDebug(void);

	//状態遷移時の処理
	void EnterStay(void);
	void EnterMove(void);
	void EnterAttackReady(void);
	void EnterAttack(void);
	void EnterReturn(void);

	//状態ごとの更新
	void UpdateStay(void);
	void UpdateMove(void);
	void UpdateAttackReady(void);
	void UpdateAttack(void);
	void UpdateReturn(void);

	//状態抜けの処理
	void ExitStay(void);
	void ExitMove(void);
	void ExitAttackReady(void);
	void ExitAttack(void);
	void ExitReturn(void);

	//方向転換
	void ChangeDir(void);

	//移動処理
	void Move(void)override;

	//攻撃処理
	void Attack(void)override;
};