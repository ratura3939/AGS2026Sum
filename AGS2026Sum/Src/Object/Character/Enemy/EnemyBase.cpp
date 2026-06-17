#include "../../../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/GameSystem/CollisionManager.h"
#include "../../../Manager/GameSystem/AttackManager.h"
#include "../../Common/Collider.h"
#include "../../Common/Geometry/Sphere.h"
#include "State/EnemyStateBase.h"
#include "State/EnemyNormalState.h"
#include "Skill/EnemySkillBase.h"
#include "Brain/EnemyBrain.h"
#include "OnHit/EnemyOnHit.h"
#include "EnemyManager.h"
#include "EnemyGroup.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const ENEMY_TYPE& _type)
	: group_(nullptr)
	, activeIndex_(-1)
	, type_(_type)
	, attackPos_(Utility::VECTOR_ZERO)
	, attackCnt_(0.0f)
	, action_(ENEMY_ACTION::STAY)
	, state_(nullptr)
	, movePow_(Utility::VECTOR_ZERO)
{	
	//行動ごとの処理の設定
	actionFunc_[static_cast<int>(ENEMY_ACTION::STAY)] = { &EnemyBase::EnterStay, &EnemyBase::UpdateStay, &EnemyBase::ExitStay };
	actionFunc_[static_cast<int>(ENEMY_ACTION::MOVE)] = { &EnemyBase::EnterMove, &EnemyBase::UpdateMove, &EnemyBase::ExitMove };
	actionFunc_[static_cast<int>(ENEMY_ACTION::ALERT)] = { &EnemyBase::EnterAlert, &EnemyBase::UpdateAlert, &EnemyBase::ExitAlert };
	actionFunc_[static_cast<int>(ENEMY_ACTION::ATTACK_READY)] = { &EnemyBase::EnterAttackReady, &EnemyBase::UpdateAttackReady, &EnemyBase::ExitAttackReady };
	actionFunc_[static_cast<int>(ENEMY_ACTION::ATTACK)] = { &EnemyBase::EnterAttack, &EnemyBase::UpdateAttack, &EnemyBase::ExitAttack };
	actionFunc_[static_cast<int>(ENEMY_ACTION::RETURN_GROUP)] = { &EnemyBase::EnterReturn, &EnemyBase::UpdateReturn, &EnemyBase::ExitReturn };

	//行動ごとの影響情報の設定
	actionInfo_[static_cast<int>(ENEMY_ACTION::STAY)] = { .isLock = false, .canMove = false };
	actionInfo_[static_cast<int>(ENEMY_ACTION::MOVE)] = { .isLock = false, .canMove = true };
	actionInfo_[static_cast<int>(ENEMY_ACTION::ALERT)] = { .isLock = false, .canMove = true };
	actionInfo_[static_cast<int>(ENEMY_ACTION::ATTACK_READY)] = { .isLock = true, .canMove = false };
	actionInfo_[static_cast<int>(ENEMY_ACTION::ATTACK)] = { .isLock = true, .canMove = false };
	actionInfo_[static_cast<int>(ENEMY_ACTION::RETURN_GROUP)] = { .isLock = false, .canMove = true };
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Draw(void)
{
	//デバッグ描画
	DrawDebug();

	//モデル描画
	MV1DrawModel(modelId_);
}

void EnemyBase::Release(void)
{
}

void EnemyBase::HitCollider(std::weak_ptr<Collider> _col)
{
	//当たり判定の処理
	onHit_->HitCollider(_col);
}

void EnemyBase::ChangeState(std::unique_ptr<EnemyStateBase> _nextState)
{
	//状態が同じなら処理しない
	if (!_nextState || state_->GetStateId() == _nextState->GetStateId())return;

	//状態抜けの処理
	if(state_) state_->Exit(*this);

	//状態の変更
	state_ = std::move(_nextState);

	//状態遷移の処理
	state_->Enter(*this);
}

void EnemyBase::ChangeAction(const ENEMY_ACTION _nextAction)
{
	//状態が同じなら処理しない
	if (action_ == _nextAction || _nextAction == ENEMY_ACTION::MAX)return;

	//状態抜けの処理
	if(action_ != ENEMY_ACTION::MAX)(this->*actionFunc_[static_cast<int>(action_)].exit)();
	
	//状態の変更
	action_ = _nextAction;
	
	//状態遷移の処理
	(this->*actionFunc_[static_cast<int>(action_)].enter)();
}

void EnemyBase::SetSkills(std::vector<std::unique_ptr<EnemySkillBase>> _skills)
{
	skills_ = std::move(_skills);
}

void EnemyBase::SetAttackCollider(std::weak_ptr<AttackDataBase> _atkData)
{
	//TODO：攻撃マネージャーに自身の名前とスキルから持ってきたデータを伝える
	//AttackManager::GetInstance().AddAttackCollider()
}

void EnemyBase::SetCurrentSkill(EnemySkillBase* _skill)
{
	//既にスキルを使っている
	if (currentSkill_)return;

	//保存
	currentSkill_ = _skill;

	//攻撃準備
	ChangeAction(ENEMY_ACTION::ATTACK_READY);
}

void EnemyBase::BreakSkill(void)
{
	//ないならスキップ
	if (!currentSkill_)return;

	//強制終了
	currentSkill_->Exit(*this);

	//破棄
	currentSkill_ = nullptr;
}

void EnemyBase::UpdateBrain(void)
{
	//思考の更新
	if (!actionInfo_[static_cast<int>(action_)].isLock)
	{
		//優先度決定
		brain_->DecidePriority();

		//行動選択
		brain_->ChoiceAction();
	}
}

void EnemyBase::UpdateAction(void)
{
	//状態ごとの更新
	(this->*actionFunc_[static_cast<int>(action_)].update)();

	//移動処理
	if (actionInfo_[static_cast<int>(action_)].canMove)Move();
}

void EnemyBase::ResetPos(void)
{
	//グループがないなら処理しない
	if(!group_) return;
	
	//グループ座標の取得
	VECTOR groupPos = group_->GetInitPos();
	VECTOR leavePos = { LEAVE_GROUP_DIST, 0.0f, LEAVE_GROUP_DIST };
	VECTOR randPos = Utility::GetRandomValue(VScale(leavePos, -1.0f), leavePos);

	//座標のリセット
	pos_ = VAdd(groupPos, randPos);
	movedPos_ = pos_;
}

void EnemyBase::DoLoad(void)
{
	//状態の初期化
	state_ = std::make_unique<EnemyNormalState>();
	state_->Enter(*this);

	//思考の初期化
	brain_ = std::make_unique<EnemyBrain>(*this);

	//接触処理の初期化
	onHit_ = std::make_unique<EnemyOnHit>(*this);
}

void EnemyBase::DoInit(void)
{
	//ローカル回転
	quaRotLocal_ = Quaternion();

	//コライダの初期化
	DeleteAllColliders();

	//当たり判定の生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, BROUD_RADIUS, RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY, { Collider::COL_TAG::PLAYER, Collider::COL_TAG::PLAYER_ATTACK });

	//攻撃コライダ
	geo = std::make_unique<Sphere>(attackPos_, attackPos_, ATTACK_BROUD_RADIUS, ATTACK_RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY_ATTACK, { Collider::COL_TAG::PLAYER });
	DisableColliderAtTag(Collider::COL_TAG::ENEMY_ATTACK);
}

void EnemyBase::InitWithGroup(void)
{
	//グループがないなら処理しない
	if (!group_)return;

	//座標
	ResetPos();
}

void EnemyBase::InitRunTimeParameter(const EnemyParameter& _param)
{
	//体力の初期化
	hp_ = _param.initHp;
}

void EnemyBase::SetModel(const int _modelId)
{
	modelId_ = _modelId;
}

void EnemyBase::SetAnim(std::unique_ptr<AnimationController> _anim)
{
	animController_ = std::move(_anim);
}

void EnemyBase::DoUpdate(void)
{
	//移動後座標に更新
	pos_ = movedPos_;

	//アニメーション更新
	animController_->Update();

	//状態の更新
	state_->Update(*this);
}

void EnemyBase::InitAnim(void)
{
}

void EnemyBase::DrawDebug(void)
{
#ifdef _DEBUG

	for (auto& col : colliders_) {
		if(col->IsUseThis())col->DrawDebugCollider();
	}

#endif // DEBUG
}

void EnemyBase::EnterStay(void)
{
	//待機アニメーションの再生
	animController_->Play(L"Idle");
}

void EnemyBase::EnterMove(void)
{
	//歩きアニメーションの再生
	animController_->Play(L"Run", RUN_ANIM_SPEED);
}

void EnemyBase::EnterAlert(void)
{
	//待機アニメーションの再生
	animController_->Play(L"Walk");
}

void EnemyBase::EnterAttackReady(void)
{
	//スキルが入ってないなら強制的に待機に移行
	if (!currentSkill_)ChangeAction(ENEMY_ACTION::STAY);

	//準備入り
	currentSkill_->ReadyEnter(*this);
}

void EnemyBase::EnterAttack(void)
{
	//スキルが入ってないなら強制的に待機に移行
	if (!currentSkill_)ChangeAction(ENEMY_ACTION::STAY);

	//攻撃入り
	currentSkill_->Enter(*this);
}

void EnemyBase::EnterReturn(void)
{
	//歩きアニメーションの再生
	animController_->Play(L"Run", RUN_ANIM_SPEED);
}

void EnemyBase::UpdateStay(void)
{
}

void EnemyBase::UpdateMove(void)
{
	//グループと一体で動く
	movePow_ = group_->GetMovePow();
}

void EnemyBase::UpdateAlert(void)
{
	//グループの目標地点に直接向かう
	VECTOR goalPos = group_->GetGoalPos();
	
	//目標地点に向かう移動量の設定
	movePow_ = Utility::GetMoveVec(pos_, goalPos, RUN_SPEED);
}

void EnemyBase::UpdateAttackReady(void)
{
	//スキルが入ってないなら強制的に待機に移行
	if (!currentSkill_)ChangeAction(ENEMY_ACTION::STAY);

	//スキルごとの準備行動
	if (currentSkill_->ReadyUpdate(*this))
	{
		//準備終了
		currentSkill_->ReadyExit(*this);

		//攻撃移行
		ChangeAction(ENEMY_ACTION::ATTACK);
	}
}

void EnemyBase::UpdateAttack(void)
{
	//スキルが入ってないなら強制的に待機に移行
	if (!currentSkill_)ChangeAction(ENEMY_ACTION::STAY);

	//スキルごとの行動
	if (currentSkill_->Update(*this))
	{
		//攻撃終了
		currentSkill_->Exit(*this);
		currentSkill_ = nullptr;

		//待機
		ChangeAction(ENEMY_ACTION::STAY);
	}
}

void EnemyBase::UpdateReturn(void)
{
	//グループ座標の取得
	VECTOR groupPos = group_->GetLeaderPos();

	//グループ座標に向かう移動量の設定
	movePow_ = Utility::GetMoveVec(pos_, groupPos, SPEED);
}

void EnemyBase::ExitStay(void)
{
}

void EnemyBase::ExitMove(void)
{
}

void EnemyBase::ExitAlert(void)
{
}

void EnemyBase::ExitAttackReady(void)
{
	//攻撃カウンタのリセット
	attackCnt_ = 0.0f;
}

void EnemyBase::ExitAttack(void)
{
	//攻撃カウンタのリセット
	attackCnt_ = 0.0f;

	//攻撃コライダの無効化
	DisableColliderAtTag(Collider::COL_TAG::ENEMY_ATTACK);
}

void EnemyBase::ExitReturn(void)
{
}

void EnemyBase::Attack(void)
{
}

void EnemyBase::Move(void)
{
	//移動後座標の更新
	VECTOR movedPos = VAdd(movedPos_, movePow_);

	//回転の更新
	quaRot_ = quaRot_.LookRotation(Utility::GetMoveVec(movedPos, movedPos_));

	//移動後座標の更新
	movedPos_ = movedPos;
}

void EnemyBase::BackMove(void)
{
	//移動後座標の更新
	VECTOR movedPos = VAdd(movedPos_, movePow_);

	//回転の更新
	quaRot_ = quaRot_.LookRotation(VScale(Utility::GetMoveVec(movedPos, movedPos_), -1.0f));

	//移動後座標の更新
	movedPos_ = movedPos;
}

void EnemyBase::EnableHitCollider(void)
{
	//当たり判定の有効化
	EnableColliderAtTag(Collider::COL_TAG::ENEMY);
}

void EnemyBase::DisableHitCollider(void)
{
	//当たり判定の無効化
	DisableColliderAtTag(Collider::COL_TAG::ENEMY);
}

void EnemyBase::SetAttackPos(const VECTOR& _localPos)
{
	//攻撃目標座標の設定
	attackPos_ = VAdd(pos_, quaRot_.PosAxis(_localPos));
}

void EnemyBase::EnableAttack(void)
{
	//攻撃コライダの有効化
	EnableColliderAtTag(Collider::COL_TAG::ENEMY_ATTACK);
}

void EnemyBase::DisableAttack(void)
{
	//攻撃コライダの無効化
	DisableColliderAtTag(Collider::COL_TAG::ENEMY_ATTACK);
}

void EnemyBase::PlayAnim(const std::wstring& _animName, const float _speed)
{
	animController_->Play(_animName, _speed);
}

void EnemyBase::PlayNoBlendAnim(const std::wstring& _animName, const float _speed)
{
	animController_->NoBlendPlay(_animName, _speed);
}

void EnemyBase::Death(void)
{
	//死亡アニメーションの再生
	animController_->Play(L"BlowEnd");
	
	//グループから離れる
	LeaveGroup();
}