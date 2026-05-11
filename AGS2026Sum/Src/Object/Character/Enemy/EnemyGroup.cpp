#include "../../../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"

EnemyGroup::EnemyGroup(const int _num)
	: pos_(Utility::VECTOR_INIT)
	,initNum_(_num)
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

	//敵の生成
	CreateEnemy();
}

void EnemyGroup::Update(void)
{
	//状態ごとの更新
	stateFunc_[state_].update();

	//敵の更新
	for(auto& enemy : enemys_)
	{
		enemy->Update();
	}

	//敵の死亡時の処理
	DeleteEnemy();
}

void EnemyGroup::Draw(void)
{
	//デバッグ
	DrawSphere3D(groupGoalPos_, 20, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	DrawSphere3D(pos_, 20, 20, GetColor(255, 255, 0), GetColor(255, 255, 0), false);

	for(auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyGroup::Release(void)
{
	for (auto& enemy : enemys_)
	{
		enemy->Release();
	}
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

void EnemyGroup::CreateEnemy(void)
{
	//敵
	std::unique_ptr<EnemyBase> enemy;

	//生成
	for (int i = 0; i < initNum_;i++)
	{
		//ランダムな初期座標を生成
		VECTOR randPos;
		randPos.x = pos_.x + static_cast<float>(Utility::GetRandomValue(-LEAVE_GROUP_DIST, LEAVE_GROUP_DIST));
		randPos.y = 0.0f;
		randPos.z = pos_.z + static_cast<float>(Utility::GetRandomValue(-LEAVE_GROUP_DIST, LEAVE_GROUP_DIST));

		//敵の生成
		enemy = std::make_unique<EnemyBase>(randPos);

		//読み込みと初期化
		enemy->Load();
		enemy->Init();

		//格納
		enemys_.push_back(std::move(enemy));
	}	
}

void EnemyGroup::DeleteEnemy(void)
{
	//そもそも敵がいないなら何もしない
	if (enemys_.empty()) return;

	//死亡した敵の削除
	std::erase_if(enemys_, [this](std::unique_ptr<EnemyBase>& _enemy) {return !_enemy->IsAlive(); });
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

	//敵の移動
	for (auto& enemy : enemys_)
	{
		if (Utility::Distance(enemy->GetPos(), pos_) > LEAVE_GROUP_DIST)
		{
			//グループから離れすぎた敵はグループに戻す
			enemy->SetMovePow(Utility::GetMoveVec(enemy->GetPos(), pos_, SPEED));
			enemy->ChangeState(EnemyBase::ENEMY_STATE::MOVE);
		}
		else
		{
			//グループにいる敵はグループの移動量を設定
			enemy->SetMovePow(movePow_);
		}
	}
}

void EnemyGroup::EnterStay(void)
{
	//行動カウンタの初期化
	actionCnt_ = 0.0f;

	//敵の状態を移動に変更
	for (auto& enemy : enemys_)
	{
		enemy->ChangeState(EnemyBase::ENEMY_STATE::STAY);
	}
}

void EnemyGroup::EnterMove(void)
{
	//行動カウンタの初期化
	actionCnt_ = 0.0f;

	//移動方向の設定
	MoveToGoal();

	//敵の状態を移動に変更
	for (auto& enemy : enemys_)
	{
		enemy->ChangeState(EnemyBase::ENEMY_STATE::MOVE);
	}
}

void EnemyGroup::EnterAttackReady(void)
{
	//行動カウンタの初期化
	actionCnt_ = 0.0f;

	//敵の状態を攻撃準備に変更
	for (auto& enemy : enemys_)
	{
		enemy->ChangeState(EnemyBase::ENEMY_STATE::ATTACK_READY);
	}
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
	for (auto& enemy : enemys_)
	{
		//敵とグループの目的地が攻撃距離より遠いなら
		if (Utility::Distance(enemy->GetPos(), groupGoalPos_) > ATTACK_DISTANCE)continue;

		//攻撃状態に遷移
		enemy->ChangeState(EnemyBase::ENEMY_STATE::ATTACK);
	}

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
