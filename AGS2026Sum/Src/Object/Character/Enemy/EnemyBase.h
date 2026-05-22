#pragma once
#include "../CharacterBase.h"
#include"EnemyGroup.h"

class EnemyBrain;

class EnemyBase : public CharacterBase
{
public:

	//敵がグループから離れられる距離
	static constexpr float LEAVE_GROUP_DIST = 1000.0f;

	//敵の状態
	enum class ENEMY_STATE
	{
		NORMAL				//通常
		, DAMAGE			//ダメージ
		, DEATH				//死亡
		, MAX
	};

	//敵個々の行動
	enum class ENEMY_ACTION
	{
		STAY				//待機
		, MOVE				//移動
		, ATTACK_READY		//攻撃準備
		, ATTACK			//攻撃
		, RETURN_GROUP		//グループに戻る
		, MAX
	};

	//コンストラクタ
	EnemyBase(void);

	//デストラクタ
	~EnemyBase(void);

	//グループとの初期化
	void InitWithGroup(void);

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//衝突後の処理
	void HitCollider(std::weak_ptr<Collider> _col)override;

	//状態遷移
	void ChangeAction(const int _nextAction);

	//生存判定用番号
	const int GetActiveIndex(void)const { return activeIndex_; }

	//生存判定用番号の設定
	void SetActiveIndex(const int _index) { activeIndex_ = _index; }

	//移動量の設定
	void SetMovePow(const VECTOR& _movePow) { movePow_ = _movePow; }

	//位置リセット
	void ResetPos(void);

	//グループに所属しているか
	const bool IsInGroup(void)const { return group_ != nullptr; }

	//所属グループの取得
	const EnemyGroup* GetGroup(void)const { return group_; }

	//敵グループの設定
	void SetGroup(const EnemyGroup* _group) { group_ = _group; }

private:

	//行動ごとの処理
	struct ActionFunc
	{
		std::function<void(void)> enter = []() {};	//行動遷移時の処理
		std::function<void(void)> update = []() {};	//更新
		std::function<void(void)> exit = []() {};	//行動抜けの処理
	};

	//当たり判定
	static constexpr float RADIUS = 30.0f;
	static constexpr float BROUD_RADIUS = RADIUS + 15.0f;

	//攻撃距離
	static constexpr float ATTACK_RADIUS = RADIUS + 15.0f;
	static constexpr float ATTACK_BROUD_RADIUS = ATTACK_RADIUS + 15.0f;
	static constexpr VECTOR ATTACK_LOCAL_POS = { 0.0f, 0.0f, 30.0f };

	//速度
	static constexpr float SPEED = 2.0f;					//移動速度
	static constexpr float RUN_SPEED = SPEED * 2.0f;		//走り速度

	//アニメーション
	static constexpr float RUN_ANIM_SPEED = 2.0f;			//走り速度

	//自身の生存判定用番号
	int activeIndex_;

	//親グループ(Managerからの参照用)
	const EnemyGroup* group_;

	//攻撃目標座標
	VECTOR attackPos_;

	//個人の移動量
	VECTOR movePow_;

	//行動
	int action_;																//行動の状態
	std::array<ActionFunc, static_cast<int>(ENEMY_ACTION::MAX)> actionFunc_;	//行動ごとの処理

	//グループの命令ごとの判断
	std::unique_ptr<EnemyBrain> brain_;

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

	//グループの命令ごとの判断
	void OrderStayChoise(void);

	//距離での状態更新
	void DistanceAction(void);

	//移動処理
	void Move(void)override;

	//攻撃処理
	void Attack(void)override;
};