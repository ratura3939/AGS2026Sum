#include "../../../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"

EnemyGroup::EnemyGroup(void)
	: pos_(Utility::VECTOR_INIT)
	, actionCnt_(0.0f)
	, movePow_(Utility::VECTOR_ZERO)
	, state_(GROUP_STATE::NONE)
{
	//状態ごとの処理の設定
	stateFunc_[GROUP_STATE::NONE] = {};
	stateFunc_[GROUP_STATE::STAY] = { [this](void) {EnterStay(); }, [this](void) {UpdateStay(); }, [this](void) {ExitStay(); } };
	stateFunc_[GROUP_STATE::MOVE] = { [this](void) {EnterMove(); }, [this](void) {UpdateMove(); }, [this](void) {ExitMove(); } };
	stateFunc_[GROUP_STATE::ATTACK_READY] = { [this](void) {EnterAttackReady(); }, [this](void) {UpdateAttackReady(); }, [this](void) {ExitAttackReady(); } };
}

EnemyGroup::~EnemyGroup(void)
{
}

void EnemyGroup::Init(void)
{
	//座標の初期化
	pos_ = Utility::VECTOR_ZERO;
	
	//行動カウントの初期化
	actionCnt_ = 0.0f;

	//状態の初期化
	ChangeState(GROUP_STATE::STAY);
}

void EnemyGroup::Update(void)
{
	//状態ごとの更新
	stateFunc_[state_].update();

	//敵の死亡時の処理
	DeleteEnemy();
}

void EnemyGroup::Draw(void)
{
	//デバッグ
	DrawSphere3D(groupGoalPos_, 20, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	DrawSphere3D(pos_, 20, 20, GetColor(255, 255, 0), GetColor(255, 255, 0), false);
}

void EnemyGroup::Release(void)
{
}

void EnemyGroup::ChangeState(const GROUP_STATE _nextState)
{
	//すでにその状態なら何もしない
	if (state_ == _nextState || _nextState == GROUP_STATE::NONE) return;

	//状態抜けの処理
	stateFunc_[state_].exit();

	//状態の変更
	state_ = _nextState;

	//状態遷移時の処理
	stateFunc_[state_].enter();
}

void EnemyGroup::ResetPos(void)
{

}

void EnemyGroup::DeleteEnemy(void)
{
	//そもそも敵がいないなら何もしない
	if (enemys_.empty()) return;

	//死亡した敵の削除
	std::erase_if(enemys_, [this](EnemyBase* _enemy) {return !_enemy->IsAlive(); });
}

void EnemyGroup::MoveToGoal(void)
{
	//移動量の設定
	movePow_ = Utility::GetMoveVec(pos_, groupGoalPos_, SPEED);
	movePow_.y = 0.0f;
}

void EnemyGroup::GroupMove(void)
{
	//ある程度近づいたならスキップ
	if (Utility::Distance(pos_, groupGoalPos_) < SPEED)return;

	//グループ座標の更新
	pos_ = VAdd(pos_, movePow_);
}

void EnemyGroup::EnterStay(void)
{
	//行動カウンタの初期化
	actionCnt_ = 0.0f;
}

void EnemyGroup::EnterMove(void)
{
	//行動カウンタの初期化
	actionCnt_ = 0.0f;

	//移動方向の設定
	MoveToGoal();
}

void EnemyGroup::EnterAttackReady(void)
{
	//行動カウンタの初期化
	actionCnt_ = 0.0f;
}

void EnemyGroup::UpdateStay(void)
{
}

void EnemyGroup::UpdateMove(void)
{
	if (actionCnt_ < ACTION_INTERVAL)
	{
		//カウンタ
		actionCnt_ += SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		//ゴール地点に向かう
		MoveToGoal();

		//カウンタの初期化
		actionCnt_ = 0.0f;
	}

	//グループ移動
	GroupMove();
}

void EnemyGroup::UpdateAttackReady(void)
{
	//ゴール地点に向かう
	MoveToGoal();

	//グループ移動
	GroupMove();
}

void EnemyGroup::ExitStay(void)
{
}

void EnemyGroup::ExitMove(void)
{
}

void EnemyGroup::ExitAttackReady(void)
{
}
