#include "../../../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "EnemyBase.h"
#include "EnemyGroup.h"

EnemyGroup::EnemyGroup(void)
	: chunkIndex_(-1)
	, actionCnt_(0.0f)
	, pos_(Utility::VECTOR_INIT)
	, initPos_(Utility::VECTOR_INIT)
	, groupGoalPos_(Utility::VECTOR_INIT)
	, movePow_(Utility::VECTOR_INIT)
	, order_(GROUP_ORDER::STAY)
	, isActive_(false)
	, isInChank_(false)
{
	//状態ごとの処理の設定
	orderFunc_[static_cast<int>(GROUP_ORDER::STAY)] = { &EnemyGroup::EnterStay, &EnemyGroup::UpdateStay, &EnemyGroup::ExitStay };
	orderFunc_[static_cast<int>(GROUP_ORDER::MOVE)] = { &EnemyGroup::EnterMove, &EnemyGroup::UpdateMove, &EnemyGroup::ExitMove };
	orderFunc_[static_cast<int>(GROUP_ORDER::ALERT)] = { &EnemyGroup::EnterAlert, &EnemyGroup::UpdateAlert, &EnemyGroup::ExitAlert };
}

EnemyGroup::~EnemyGroup(void)
{
}

void EnemyGroup::Init(const VECTOR& _initPos)
{
	//初期座標保存
	initPos_ = _initPos;
	pos_ = initPos_;

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
	(this->*orderFunc_[static_cast<int>(order_)].update)();

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
	//DrawSphere3D(groupGoalPos_, 20, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	//DrawSphere3D(pos_, 20, 20, GetColor(255, 255, 0), GetColor(255, 255, 0), false);

	//敵の描画
	for (auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyGroup::Release(void)
{
}

void EnemyGroup::OnEnterActiveChank(void)
{
	//チャンクに入った
	isInChank_ = true;

	//各敵の加入処理
	for (auto& enemy : enemys_)
	{
		enemy->OnEnterActiveChank();
	}
}

void EnemyGroup::OnLeaveActiveChank(void)
{
	//チャンクから出た
	isInChank_ = false;

	//各敵の離脱処理
	for (auto& enemy : enemys_)
	{
		enemy->OnLeaveActiveChank();
	}
}

const VECTOR& EnemyGroup::GetGroupPos(void)const
{
	return pos_;
}

void EnemyGroup::ChangeOrder(const GROUP_ORDER _nextOrder)
{
	//すでにその状態なら何もしない
	if (order_ == _nextOrder) return;

	//状態抜けの処理
	if(orderFunc_[static_cast<int>(order_)].exit != nullptr)(this->*orderFunc_[static_cast<int>(order_)].exit)();

	//状態の変更
	order_ = _nextOrder;

	//状態遷移時の処理
	(this->*orderFunc_[static_cast<int>(order_)].enter)();
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
	//敵がいるか
	if (enemys_.empty()) return;

	//死亡した敵の削除
	std::erase_if(enemys_, [this](EnemyBase* _enemy) {return _enemy->IsEndState(); });

	//グループ解散の判定
	Disband();
}

void EnemyGroup::Disband(void)
{
	//ボスが存在するか
	bool isBossExist = false;
	for (auto& enemy : enemys_)
	{
		if (enemy->IsBoss())
		{
			isBossExist = true;
		}
	}

	//敵の数が少なくなった　かつ　ボスが存在しない場合はグループを解散する
	if (GetEnemyCount() < MIN_ENEMY_NUM && !isBossExist)
	{
		isActive_ = false;
		return;
	}
}

void EnemyGroup::MoveToGoal(void)
{
	//移動量の設定
	movePow_ = Utility::GetMoveVec(pos_, groupGoalPos_, SPEED);
	movePow_.y = 0.0f;
}

void EnemyGroup::GroupMove(void)
{
	//グループ座標の移動
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
