#include "../../../pch.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/GameSystem/CollisionManager.h"
#include "../../Common/Collider.h"
#include "../../Common/Geometry/Sphere.h"
#include "State/EnemyStateBase.h"
#include "State/EnemyNormalState.h"
#include "EnemyManager.h"
#include "EnemyGroup.h"
#include "EnemyBrain.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
	: group_(nullptr)
	, activeIndex_(-1)
	, attackPos_(Utility::VECTOR_ZERO)
	, attackCnt_(0.0f)
	, brain_(*this)
	, onHit_(*this)
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
	actionInfo_[static_cast<int>(ENEMY_ACTION::STAY)] = { .isLock = false, .canMove = true };
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
	onHit_.HitCollider(_col);
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
	(this->*actionFunc_[static_cast<int>(action_)].exit)();
	
	//状態の変更
	action_ = _nextAction;
	
	//状態遷移の処理
	(this->*actionFunc_[static_cast<int>(action_)].enter)();
}

void EnemyBase::UpdateBrain(void)
{
	//思考の更新
	if (!actionInfo_[static_cast<int>(action_)].isLock)
	{
		//優先度決定
		brain_.DecidePriority();

		//行動選択
		brain_.ChoiceAction();
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
	VECTOR groupPos = group_->GetPos();
	VECTOR leavePos = { LEAVE_GROUP_DIST, 0.0f, LEAVE_GROUP_DIST };
	VECTOR randPos = Utility::GetRandomValue(VScale(leavePos, -1.0f), leavePos);

	//座標のリセット
	pos_ = VAdd(groupPos, randPos);
	movedPos_ = pos_;
}

void EnemyBase::DoLoad(void)
{
	//モデル差し込み
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MDL);

	//アニメーションの初期化
	InitAnim();

	//状態の初期化
	state_ = std::make_unique<EnemyNormalState>();
	state_->Enter(*this);
}

void EnemyBase::DoInit(void)
{
	//体力
	hp_ = 100.0f;		
	quaRotLocal_ = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	//当たり判定の生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, BROUD_RADIUS, RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY, { Collider::COL_TAG::PLAYER, Collider::COL_TAG::PLAYER_ATTACK });
}

void EnemyBase::InitWithGroup(void)
{
	//グループがないなら処理しない
	if (!group_)return;

	//座標
	ResetPos();
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
	//インスタンス取得
	auto& res = ResourceManager::GetInstance();

	//アニメーションの初期化
	animController_ = std::make_unique<AnimationController>(modelId_);

	//待機アニメーション
	int animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE_ANIM);
	animController_->Add(L"Idle", animData, AnimationController::PLAY_TYPE::LOOP, AnimationController::ANIM_SOURCE::EXTERNAL);

	//歩きアニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_WALK_ANIM);
	animController_->Add(L"Walk", animData, AnimationController::PLAY_TYPE::LOOP, AnimationController::ANIM_SOURCE::EXTERNAL);

	//走りアニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RUN_ANIM);
	animController_->Add(L"Run", animData, AnimationController::PLAY_TYPE::LOOP, AnimationController::ANIM_SOURCE::EXTERNAL);

	//攻撃アニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_ATTACK_ANIM);
	animController_->Add(L"Attack", animData, AnimationController::PLAY_TYPE::NORMAL, AnimationController::ANIM_SOURCE::EXTERNAL);

	//死亡アニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_DEATH_ANIM);
	animController_->Add(L"Death", animData, AnimationController::PLAY_TYPE::NORMAL, AnimationController::ANIM_SOURCE::EXTERNAL);

	//デフォルトアニメーションの設定
	animController_->SetDefaultAnim(L"Idle");
}

void EnemyBase::DrawDebug(void)
{
#ifdef _DEBUG

	for (auto& col : colliders_) {
		col->DrawDebugCollider();
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
}

void EnemyBase::EnterAttack(void)
{
	//攻撃処理
	Attack();
}

void EnemyBase::EnterReturn(void)
{
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
	//攻撃準備時間を超えたら攻撃状態に遷移
	if (attackCnt_ > ATTACK_READY_TIME) ChangeAction(ENEMY_ACTION::ATTACK);
	else attackCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void EnemyBase::UpdateAttack(void)
{
	//攻撃時間を超えたら待機状態に遷移
	if (attackCnt_ > ATTACK_TIME) ChangeAction(ENEMY_ACTION::STAY);
	else attackCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void EnemyBase::UpdateReturn(void)
{
	//グループ座標の取得
	VECTOR groupPos = group_->GetPos();

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

	//攻撃コライダの削除
	DeleteColliderAtTag(Collider::COL_TAG::ENEMY_ATTACK);
}

void EnemyBase::ExitReturn(void)
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

void EnemyBase::Attack(void)
{
	//攻撃目標座標の設定
	attackPos_ = VAdd(pos_, quaRot_.PosAxis(ATTACK_LOCAL_POS));

	//攻撃コライダ
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(attackPos_, attackPos_, ATTACK_BROUD_RADIUS, ATTACK_RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY_ATTACK, { Collider::COL_TAG::PLAYER });

	//攻撃アニメーションの再生
	animController_->Play(L"Attack");
}