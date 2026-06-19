#include "../../../../pch.h"
#include"../../../../Manager/GameSystem/AttackManager.h"
#include "../EnemyBase.h"
#include "../State/EnemyDamageState.h"
#include "EnemyOnHit.h"

EnemyOnHit::EnemyOnHit(EnemyBase& _parent)
	: parent_(_parent)
{
}

EnemyOnHit::~EnemyOnHit(void)
{
}

void EnemyOnHit::CalcDamage(const std::weak_ptr<Collider> _col)
{
	//ヒット者
	const auto& col = _col.lock();

	//攻撃情報
	const std::weak_ptr<AttackDataBase>& data = AttackManager::GetInstance().GetAttackData(_col);

	//回転情報
	const Quaternion quaRot = col->GetGeometry().GetColRot();

	//吹っ飛び(相手の向いている方向)
	VECTOR blowPow = VScale(quaRot.GetForward(), -BLOW_POWER);

	//ダメージ状態
	parent_.ChangeState(std::make_unique<EnemyDamageState>());

	//ダメージ処理
	parent_.Damage(data.lock()->power);
}

void EnemyOnHit::HitPlayer(const std::weak_ptr<Collider> _col)
{
}

void EnemyOnHit::HitPlayerAttack(const std::weak_ptr<Collider> _col)
{
	//ダメージ処理
	CalcDamage(_col);
}

void EnemyOnHit::HitEnemy(const std::weak_ptr<Collider> _col)
{
	//相手のコライダ
	const auto& hitCol = _col.lock();

	//相手の形状
	const auto& geo = hitCol->GetGeometry();

	//相手側の接触情報
	const auto& result = geo.GetHitResult();

	//自分の法線方向なので相手側の逆
	VECTOR myNormal = VScale(result.normal, -1.0f);

	//移動後座標
	VECTOR movedPos = parent_.GetMovedPos();

	//戻す移動量
	VECTOR movePow = VSub(movedPos, parent_.GetPos());

	//接触地点に戻す
	movedPos = VAdd(movedPos, VScale(movePow, result.t));

	//めり込み防止用にもう少し戻す
	movedPos = VAdd(movedPos, VScale(myNormal, 0.01f));

	//移動量分も押し戻す
	movedPos = VAdd(movedPos, VScale(movePow, -1.0f));

	//残りをスライドさせる
	float remain = 1.0f - result.t;
	VECTOR slide = VSub(movePow, VScale(myNormal, VDot(movePow, myNormal)));
	parent_.SetMovedPos(VAdd(movedPos, VScale(slide, remain)));
}

void EnemyOnHit::HitEnemyAttack(const std::weak_ptr<Collider> _col)
{
}
