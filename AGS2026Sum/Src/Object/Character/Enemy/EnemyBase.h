#pragma once
#include <array>
#include <string>
#include "../../../Manager/GameSystem/AttackManager.h"
#include "../../../Manager/GameSystem/Event/EventType.h"
#include "../CharacterBase.h"
#include "Info/EnemyDefine.h"
#include "Info/EnemyParameter.h"
#include"EnemyGroup.h"

class EnemyStateBase;
class EnemySkillBase;
class BrainBase;
class EnemyOnHit;
 
class EnemyBase : public CharacterBase
{
public:

	//敵がグループから離れられる距離
	static constexpr float LEAVE_GROUP_DIST = 500.0f;

	//当たり判定
	static constexpr float RADIUS = 30.0f;
	static constexpr float BROUD_RADIUS = RADIUS + 15.0f;

	//攻撃距離
	static constexpr float ATTACK_RADIUS = RADIUS + 100.0f;
	static constexpr float ATTACK_BROUD_RADIUS = ATTACK_RADIUS + 15.0f;

	//攻撃持続時間
	static constexpr float ATTACK_DURATION = 1.0f;

	//シェーダーの設定
	static constexpr int PS_C_BUFF_SIZE = 1;
	static constexpr int BUFFER_MATRIX_SIZE = 2;

	//コンストラクタ
	EnemyBase(const ENEMY_TYPE& _type);

	//デストラクタ
	virtual~EnemyBase(void)override;

	//グループとの初期化
	void InitWithGroup(void);

	//通常パラメーターの初期化
	void InitRunTimeParameter(const EnemyParameter& _param);

	//モデル設定
	void SetModel(const int _modelId);

	//アニメーション設定
	void SetAnim(std::unique_ptr<AnimationController> _anim);

	//描画
	virtual void Draw(void)override;

	//解放
	void Release(void)override;

	//衝突後の処理
	void HitCollider(std::weak_ptr<Collider> _col)override;

	//状態遷移
	void ChangeState(std::unique_ptr<EnemyStateBase> _nextState);

	//行動遷移
	void ChangeAction(const ENEMY_ACTION _nextAction);

	//所持スキル取得
	const std::vector<std::unique_ptr<EnemySkillBase>>& GetSkills(void) { return skills_; }

	//所持スキル設定
	void SetSkills(std::vector<std::unique_ptr<EnemySkillBase>> _skills);

	//攻撃マネージャーにコライダを設定
	void SetAttackCollider(const AttackManager::ATTACK_TYPE& _name);

	//攻撃マネージャーからコライダを破棄
	void RemoveAttackCollider(void);

	//現在の攻撃取得
	const EnemySkillBase* GetCurrentSkill(void)const { return currentSkill_; };

	//現在の攻撃設定
	void SetCurrentSkill(EnemySkillBase* _skill);

	//現在の攻撃の破棄
	void RemoveCurrentSkill(void);

	//思考の更新
	void UpdateBrain(void);

	//行動の更新
	void UpdateAction(void);

	//生存判定用番号
	const int GetActiveIndex(void)const { return activeIndex_; }

	//生存判定用番号の設定
	void SetActiveIndex(const int _index) { activeIndex_ = _index; }

	//敵の種類の取得
	const ENEMY_TYPE& GetType(void) { return type_; }

	//ボスかどうか
	const bool IsBoss(void)const { return type_ != ENEMY_TYPE::NORMAL; }

	//移動量の取得
	const VECTOR& GetMovePow(void)const { return movePow_; }

	//移動量の設定
	void SetMovePow(const VECTOR& _movePow) { movePow_ = _movePow; }

	//重力の取得
	const VECTOR& GetGravityPow(void)const { return gravityPow_; }

	//重力の設定
	void SetGravityPow(const VECTOR& _gravityPow) { gravityPow_ = _gravityPow; }

	//位置リセット
	void ResetPos(void);

	//グループを抜ける処理
	void StateEnd(void);

	//グループに所属しているか
	const bool IsInGroup(void)const { return group_ != nullptr; }

	//所属グループの取得
	const EnemyGroup* GetGroup(void)const { return group_; }

	//敵グループの設定
	void SetGroup(const EnemyGroup* _group) { group_ = _group; }

	//移動処理
	void Move(void)override;

	//回転更新
	void Rotation(void);

	//速度設定
	void SetSpeed(const float _speed) { speed_ = _speed; }

	//移動量を更新
	void UpdateMovePow(void);

	//バック移動
	void BackMove(void);

	//バック用回転更新
	void BackRotation(void);

	//吹っ飛び
	void Knockback(void);

	//本体当たり判定の有効化
	void EnableHitCollider(void);

	//本体当たり判定の無効化
	void DisableHitCollider(void);

	//接地判定
	const bool IsGrounding(void)const { return isGrounding_; };

	//接地状態の設定
	void SetGrounding(const bool _isGrounding) { isGrounding_ = _isGrounding; }

	//本体当たり判定の半径
	const float GetHitRadius(void);

	//攻撃座標の設定
	void SetAttackPos(const VECTOR& _localPos);

	//攻撃範囲の設定
	void SetAttackRadius(const float _radius);

	//攻撃の有効化
	void EnableAttack(void);

	//攻撃の無効化
	void DisableAttack(void);

	//特殊スキルの有無
	void SetIsElementSkill(const bool _value) { isElementSkill_ = _value; }

	//アニメーションプレイ
	void PlayAnim(const std::wstring& _animName, const float _speed = 1.0f);
	void PlayAnimIsFinish(const std::wstring& _animName, const float _speed = 1.0f);
	void PlayNoBlendAnim(const std::wstring& _animName, const float _speed = 1.0f);
	void PlayNoBlendAnimIsFinish(const std::wstring& _animName, const float _speed = 1.0f);

	//フェード中か
	const bool IsFade(void)const;

	//終了状態か
	const bool IsEndState(void)const;

	//イベント管理
	void SetEventKey(const EVENT_TYPE& _event);

	//イベントフラグ管理の設定
	void AddEventCount(void)const;

	//イベントフラグの判定
	void SubEventCount(void)const;

	//チャンクに入った時の処理
	void OnEnterActiveChank(void);

	//チャンクから出た時の処理
	void OnLeaveActiveChank(void);

	//色変更
	void SetColor(FLOAT4 _color) { color_ = _color; }

	//ガードブレイクしているか
	virtual const bool IsGuardBreak(void)const { return false; }

protected:

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
	static constexpr float ALERT_SPEED = 6.0f;					//警戒時速度
	static constexpr float WALK_SPEED = ALERT_SPEED * 2.0f;		//通常移動速度
	static constexpr float RETURN_SPEED = ALERT_SPEED * 3.0f;	//通常移動速度

	//アニメーション
	static constexpr float RUN_ANIM_SPEED = 2.0f;		//走り速度

	//攻撃時間
	static constexpr float ATTACK_READY_TIME = 1.0f;	//攻撃準備時間

	//自身の生存判定用番号
	int activeIndex_;

	//体力
	float hpMax_;

	//敵のタイプ
	const ENEMY_TYPE type_;

	//親グループ(Managerからの参照用)
	const EnemyGroup* group_;

	//攻撃目標座標
	VECTOR attackPos_;

	//個人の移動量
	VECTOR movePow_;
	VECTOR gravityPow_;

	//目標地点
	VECTOR goalPos_;

	//移動速度
	float speed_;

	//接地
	bool isGrounding_;

	//状態
	std::unique_ptr<EnemyStateBase> state_;

	//行動
	ENEMY_ACTION action_;														//行動の状態
	std::array<ActionFunc, static_cast<int>(ENEMY_ACTION::MAX)> actionFunc_;	//行動ごとの処理
	std::array<ActionInfo, static_cast<int>(ENEMY_ACTION::MAX)> actionInfo_;	//行動ごとの影響情報

	//攻撃スキル
	EnemySkillBase* currentSkill_;
	
	//所持スキル
	std::vector<std::unique_ptr<EnemySkillBase>> skills_;

	//特殊スキル開始の有無
	bool isElementSkill_;

	//グループの命令ごとの判断
	std::unique_ptr<BrainBase> brain_;

	//当たり判定の処理
	std::unique_ptr<EnemyOnHit> onHit_;

	//発動イベント情報
	EVENT_TYPE eventKey_;

	//色
	FLOAT4 color_;

	//読み込み
	virtual void DoLoad(void)override;

	//シェーダーの読み込み
	virtual void LoadShader(void);

	//初期化
	virtual void DoInit(void)override;

	//更新
	virtual void DoUpdate(void)override;

	//コライダの生成
	virtual void CreateCollider(void);

	//コライダの全削除(Groupからの命令用)
	void DeleteCollider(void);

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
	void EnterAttackEnd(void);
	void EnterReturn(void);

	//行動ごとの更新
	void UpdateStay(void);
	void UpdateMove(void);
	void UpdateAlert(void);
	void UpdateAttackReady(void);
	void UpdateAttack(void);
	void UpdateAttackEnd(void);
	void UpdateReturn(void);

	//行動抜けの処理
	void ExitStay(void);
	void ExitMove(void);
	void ExitAlert(void);
	void ExitAttackReady(void);
	void ExitAttack(void);
	void ExitAttackEnd(void);
	void ExitReturn(void);

	//攻撃
	void Attack(void)override;

	//死亡処理
	void Death(void)override;
};