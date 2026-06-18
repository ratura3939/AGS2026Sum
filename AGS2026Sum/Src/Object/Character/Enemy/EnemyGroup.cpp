#include "../../../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"

EnemyGroup::EnemyGroup(void)
	: chunkIndex_(-1)
	, actionCnt_(0.0f)
	, movePow_(Utility::VECTOR_ZERO)
	, order_(GROUP_ORDER::NONE)
	, isActive_(false)
{
	//状態ごとの処理の設定
	orderFunc_[GROUP_ORDER::NONE] = {};
	orderFunc_[GROUP_ORDER::STAY] = { &EnemyGroup::EnterStay, &EnemyGroup::UpdateStay, &EnemyGroup::ExitStay };
	orderFunc_[GROUP_ORDER::MOVE] = { &EnemyGroup::EnterMove, &EnemyGroup::UpdateMove, &EnemyGroup::ExitMove };
	orderFunc_[GROUP_ORDER::ALERT] = { &EnemyGroup::EnterAlert, &EnemyGroup::UpdateAlert, &EnemyGroup::ExitAlert };
}

EnemyGroup::~EnemyGroup(void)
{
	//TODO:敵にグループ崩壊を伝える
	for(EnemyBase* enemy : enemys_)
	{
		enemy->LeaveGroup();
	}
}

void EnemyGroup::Init(const VECTOR& _initPos)
{
	//初期座標保存
	initPos_ = { 1000.0f,0.0f,1000.0f };

	//生存判定の初期化
	isActive_ = true;
	
	//行動カウントの初期化
	actionCnt_ = 0.0f;

	//状態の初期化
	ChangeOrder(GROUP_ORDER::STAY);
}

void EnemyGroup::Update(void)
{
	//状態ごとの更新
	(this->*orderFunc_[order_].update)();

	//敵の更新
	for (auto& enemy : enemys_)
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
	DrawSphere3D(leader_->GetPos(), 20, 20, GetColor(255, 255, 0), GetColor(255, 255, 0), false);

	//敵の描画
	for (auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyGroup::Release(void)
{
}

const VECTOR& EnemyGroup::GetLeaderPos(void)const
{
	return leader_->GetPos();
}

void EnemyGroup::SetLeader(const EnemyBase* _leader)
{
	//リーダー設定
	leader_ = _leader;
}

void EnemyGroup::ChangeOrder(const GROUP_ORDER _nextOrder)
{
	//すでにその状態なら何もしない
	if (order_ == _nextOrder || _nextOrder == GROUP_ORDER::NONE) return;

	//状態抜けの処理
	if(orderFunc_[order_].exit)(this->*orderFunc_[order_].exit)();

	//状態の変更
	order_ = _nextOrder;

	//状態遷移時の処理
	(this->*orderFunc_[order_].enter)();
}

void EnemyGroup::ResetPos(void)
{
	for (auto& enemy : enemys_)
	{
		enemy->ResetPos();
	}
}

void EnemyGroup::DeleteEnemy(void)
{
	//そもそも敵がいないなら何もしない
	if (enemys_.empty()) return;

	//リーダーの死亡判定
	bool isDeadLeader = false;
	if (!leader_ || !leader_->IsAlive())
	{
		//死んだ
		isDeadLeader = true;
		leader_ = nullptr;
	}

	//死亡した敵の削除
	std::erase_if(enemys_, [this](EnemyBase* _enemy) {return !_enemy->IsAlive(); });

	//リーダーの再設定
	if (isDeadLeader)ResetLeader();
}

void EnemyGroup::ResetLeader(void)
{
	//先頭をリーダーに
	leader_ = enemys_.front();
}

void EnemyGroup::MoveToGoal(void)
{
	//移動量の設定
	movePow_ = Utility::GetMoveVec(leader_->GetPos(), groupGoalPos_, SPEED);
	movePow_.y = 0.0f;
}

void EnemyGroup::GroupMove(void)
{
	//各敵で処理
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

void EnemyGroup::EnterAlert(void)
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

void EnemyGroup::UpdateAlert(void)
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

void EnemyGroup::ExitAlert(void)
{
}
