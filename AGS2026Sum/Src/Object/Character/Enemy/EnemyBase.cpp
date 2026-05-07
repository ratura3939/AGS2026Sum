#include "../../../pch.h"
#include "../../../Manager/GameSystem/CollisionManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../Common/Collider.h"
#include "../../Common/Geometry/Sphere.h"
#include "EnemyGroup.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const VECTOR& _initPos, const VECTOR& _movePow)
	: initPos_(_initPos)
	, groupMovePow_(_movePow)
	, state_(ENEMY_STATE::NONE)
{
	//状態ごとの処理の設定
	stateFunc_[ENEMY_STATE::NONE] = {};
	stateFunc_[ENEMY_STATE::STAY] = { [this](void) {EnterStay(); }, [this](void) {UpdateStay(); }, [this](void) {ExitStay(); } };
	stateFunc_[ENEMY_STATE::MOVE] = { [this](void) {EnterMove(); }, [this](void) {UpdateMove(); }, [this](void) {ExitMove(); } };
	stateFunc_[ENEMY_STATE::ATTACK_READY] = { [this](void) {EnterAttackReady(); }, [this](void) {UpdateAttackReady(); }, [this](void) {ExitAttackReady(); } };
	stateFunc_[ENEMY_STATE::ATTACK] = { [this](void) {EnterAttack(); }, [this](void) {UpdateAttack(); }, [this](void) {ExitAttack(); } };
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

void EnemyBase::ChangeState(const ENEMY_STATE _nextState)
{
	//状態が同じなら処理しない
	if (state_ == _nextState || _nextState == ENEMY_STATE::NONE)return;

	//状態抜けの処理
	stateFunc_[state_].exit();
	
	//状態の変更
	state_ = _nextState;
	
	//状態遷移の処理
	stateFunc_[state_].enter();
}

void EnemyBase::DoLoad(void)
{
	//モデル差し込み
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MDL);
}

void EnemyBase::DoInit(void)
{
	//体力
	hp_ = 100.0f;		
	quaRotLocal_ = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	//座標
	pos_ = initPos_;
	movedPos_ = initPos_;

	//当たり判定の生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, BROUD_RADIUS, RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY, { Collider::COL_TAG::PLAYER, Collider::COL_TAG::PLAYER_ATTACK });
}

void EnemyBase::DoUpdate(void)
{
	//移動後座標に更新
	pos_ = movedPos_;

	//状態ごとの更新
	stateFunc_[state_].update();
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
}

void EnemyBase::EnterMove(void)
{
}

void EnemyBase::EnterAttackReady(void)
{
}

void EnemyBase::EnterAttack(void)
{
	//攻撃目標座標の設定
	attackPos_ = quaRot_.PosAxis(VAdd(pos_, ATTACK_LOCAL_POS));

	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, ATTACK_BROUD_RADIUS, ATTACK_RADIUS);
}

void EnemyBase::UpdateStay(void)
{
}

void EnemyBase::UpdateMove(void)
{
	//移動処理
	Move();
}

void EnemyBase::UpdateAttackReady(void)
{
	//移動処理
	Move();
}

void EnemyBase::UpdateAttack(void)
{
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

void EnemyBase::Move(void)
{
	VECTOR movedPos = VAdd(movedPos_, groupMovePow_);
	quaRot_ = quaRot_.LookRotation(Utility::GetMoveVec(movedPos, movedPos_));
	movedPos_ = movedPos;
}

void EnemyBase::Attack(void)
{
}
