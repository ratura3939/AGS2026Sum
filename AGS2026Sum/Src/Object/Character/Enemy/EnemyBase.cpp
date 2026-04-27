#include "../../../pch.h"
#include "../../../Manager/GameSystem/CollisionManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "EnemyGroup.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const VECTOR& _initPos, const VECTOR& _movePow)
	: initPos_(_initPos)
	, movePow_(_movePow)
{
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
}

void EnemyBase::DoUpdate(void)
{
	//移動後座標に更新
	pos_ = movedPos_;

	//移動処理
	Move();
}

void EnemyBase::DrawDebug(void)
{
#ifdef _DEBUG

	//for (auto& col : colliders_) {
	//	col->DrawDebugCollider();
	//}

#endif // DEBUG
}

void EnemyBase::Move(void)
{
	VECTOR movedPos = VAdd(movedPos_, movePow_);
	quaRot_ = quaRot_.LookRotation(Utility::GetMoveVec(movedPos, movedPos_));
	movedPos_ = movedPos;
}

void EnemyBase::Attack(void)
{
}
