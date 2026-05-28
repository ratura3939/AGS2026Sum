#pragma once
#include <array>
#include <string>
#include "../CharacterBase.h"
#include "EnemyDefine.h"
#include"EnemyBrain.h"
#include"EnemyOnHit.h"
#include"EnemyGroup.h"

class EnemyStateBase;
 
class EnemyBase : public CharacterBase
{
public:

	//敵がグループから離れられる距離
	static constexpr float LEAVE_GROUP_DIST = 1000.0f;

	//当たり判定
	static constexpr float RADIUS = 30.0f;
	static constexpr float BROUD_RADIUS = RADIUS + 15.0f;

	//攻撃距離
	static constexpr float ATTACK_RADIUS = RADIUS + 15.0f;
	static constexpr float ATTACK_BROUD_RADIUS = ATTACK_RADIUS + 15.0f;
	static constexpr VECTOR ATTACK_LOCAL_POS = { 0.0f, 0.0f, -30.0f };

	//攻撃持続時間
	static constexpr float ATTACK_DURATION = 1.0f;

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
	void ChangeState(std::unique_ptr<EnemyStateBase> _nextState);

	//行動遷移
	void ChangeAction(const ENEMY_ACTION _nextAction);

	//思考の更新
	void UpdateBrain(void);

	//行動の更新
	void UpdateAction(void);

	//生存判定用番号
	const int GetActiveIndex(void)const { return activeIndex_; }

	//生存判定用番号の設定
	void SetActiveIndex(const int _index) { activeIndex_ = _index; }

	//移動量の取得
	const VECTOR& GetMovePow(void)const { return movePow_; }

	//移動量の設定
	void SetMovePow(const VECTOR& _movePow) { movePow_ = _movePow; }

	//位置リセット
	void ResetPos(void);

	//グループを抜ける
	void LeaveGroup(void) { group_ = nullptr; }

	//グループに所属しているか
	const bool IsInGroup(void)const { return group_ != nullptr; }

	//所属グループの取得
	const EnemyGroup* GetGroup(void)const { return group_; }

	//敵グループの設定
	void SetGroup(const EnemyGroup* _group) { group_ = _group; }

	//移動処理
	void Move(void)override;

	//バック移動
	void BackMove(void);

	//アニメーションプレイ
	void PlayAnim(const std::wstring& _animName, const float _speed = 1.0f);

private:

	//親ボーン名
	static const std::wstring ROOT_NAME;

	//行動の関数ポインタ
	using Func = void(EnemyBase::*)(void);

	//行動影響情報
	struct ActionInfo
	{
		bool isLock = false;		//行動ロック
		bool canMove = false;		//移動可能か
	};

	//行動ごとの処理
	struct ActionFunc
	{
		Func enter = nullptr;	//行動遷移時の処理
		Func update = nullptr;	//更新
		Func exit = nullptr;	//行動抜けの処理
	};

	//速度
	static constexpr float SPEED = 2.0f;				//移動速度
	static constexpr float RUN_SPEED = SPEED * 2.0f;	//走り速度

	//アニメーション
	static constexpr float RUN_ANIM_SPEED = 2.0f;		//走り速度

	//攻撃時間
	static constexpr float ATTACK_READY_TIME = 1.0f;	//攻撃準備時間
	static constexpr float ATTACK_TIME = 3.0f;			//攻撃時間

	//自身の生存判定用番号
	int activeIndex_;

	//親グループ(Managerからの参照用)
	const EnemyGroup* group_;

	//攻撃目標座標
	VECTOR attackPos_;

	//攻撃用カウンタ
	float attackCnt_;

	//個人の移動量
	VECTOR movePow_;

	//状態
	std::unique_ptr<EnemyStateBase> state_;

	//行動
	ENEMY_ACTION action_;														//行動の状態
	std::array<ActionFunc, static_cast<int>(ENEMY_ACTION::MAX)> actionFunc_;	//行動ごとの処理
	std::array<ActionInfo, static_cast<int>(ENEMY_ACTION::MAX)> actionInfo_;	//行動ごとの影響情報

	//グループの命令ごとの判断
	EnemyBrain brain_;

	//当たり判定の処理
	EnemyOnHit onHit_;

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

	//行動遷移時の処理
	void EnterStay(void);
	void EnterMove(void);
	void EnterAlert(void);
	void EnterAttackReady(void);
	void EnterAttack(void);
	void EnterReturn(void);

	//行動ごとの更新
	void UpdateStay(void);
	void UpdateMove(void);
	void UpdateAlert(void);
	void UpdateAttackReady(void);
	void UpdateAttack(void);
	void UpdateReturn(void);

	//行動抜けの処理
	void ExitStay(void);
	void ExitMove(void);
	void ExitAlert(void);
	void ExitAttackReady(void);
	void ExitAttack(void);
	void ExitReturn(void);

	//攻撃処理
	void Attack(void)override;

	//死亡処理
	void Death(void)override;
};