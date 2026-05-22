#include "../../../pch.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/GameSystem/CollisionManager.h"
#include "../../Common/Collider.h"
#include "../../Common/Geometry/Sphere.h"
#include "EnemyManager.h"
#include "EnemyGroup.h"
#include "EnemyBrain.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
	: group_(nullptr)
	, action_(static_cast<int>(ENEMY_ACTION::STAY))
	, movePow_(Utility::VECTOR_ZERO)
{
	//状態ごとの処理の設定
	actionFunc_[static_cast<int>(ENEMY_ACTION::STAY)] = { [this](void) {EnterStay(); }, [this](void) {UpdateStay(); }, [this](void) {ExitStay(); } };
	actionFunc_[static_cast<int>(ENEMY_ACTION::MOVE)] = { [this](void) {EnterMove(); }, [this](void) {UpdateMove(); }, [this](void) {ExitMove(); } };
	actionFunc_[static_cast<int>(ENEMY_ACTION::ATTACK_READY)] = { [this](void) {EnterAttackReady(); }, [this](void) {UpdateAttackReady(); }, [this](void) {ExitAttackReady(); } };
	actionFunc_[static_cast<int>(ENEMY_ACTION::ATTACK)] = { [this](void) {EnterAttack(); }, [this](void) {UpdateAttack(); }, [this](void) {ExitAttack(); } };
	actionFunc_[static_cast<int>(ENEMY_ACTION::RETURN_GROUP)] = { [this](void) {EnterReturn(); }, [this](void) {UpdateReturn(); }, [this](void) {ExitReturn(); } };
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
}

void EnemyBase::ChangeAction(const int _nextAction)
{
	//状態が同じなら処理しない
	if (action_ == _nextAction || _nextAction < 0 || _nextAction >= static_cast<int>(ENEMY_ACTION::MAX))return;

	//状態抜けの処理
	actionFunc_[action_].exit();
	
	//状態の変更
	action_ = _nextAction;
	
	//状態遷移の処理
	actionFunc_[action_].enter();
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

	//思考の初期化
	brain_ = std::make_unique<EnemyBrain>(*this);

	//アニメーションの初期化
	InitAnim();
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

	//思考の更新
	brain_->DecidePriority();
	brain_->ChoiceAction();

	//状態ごとの更新
	actionFunc_[action_].update();
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

void EnemyBase::EnterAttackReady(void)
{
	//待機アニメーションの再生
	animController_->Play(L"Walk");
}

void EnemyBase::EnterAttack(void)
{
	//攻撃目標座標の設定
	attackPos_ = VAdd(pos_, quaRot_.PosAxis(ATTACK_LOCAL_POS));

	//攻撃コライダ
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, ATTACK_BROUD_RADIUS, ATTACK_RADIUS);

	//攻撃アニメーションの再生
	animController_->Play(L"Attack");
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

	//移動処理
	Move();
}

void EnemyBase::UpdateAttackReady(void)
{
	//グループの目標地点に直接向かう
	VECTOR goalPos = group_->GetGoalPos();
	
	//目標地点に向かう移動量の設定
	movePow_ = Utility::GetMoveVec(pos_, goalPos, RUN_SPEED);

	//移動処理
	Move();
}

void EnemyBase::UpdateAttack(void)
{
}

void EnemyBase::UpdateReturn(void)
{
	//グループ座標の取得
	VECTOR groupPos = group_->GetPos();

	//グループ座標に向かう移動量の設定
	movePow_ = Utility::GetMoveVec(pos_, groupPos, SPEED);

	//移動処理
	Move();
}

void EnemyBase::ExitStay(void)
{
}

void EnemyBase::ExitMove(void)
{
}

void EnemyBase::ExitAttackReady(void)
{
}

void EnemyBase::ExitAttack(void)
{
}

void EnemyBase::ExitReturn(void)
{
}

void EnemyBase::DistanceAction(void)
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
}
