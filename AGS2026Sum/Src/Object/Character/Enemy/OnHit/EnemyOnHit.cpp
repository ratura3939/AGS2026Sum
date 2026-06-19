#include "../../../../pch.h"
#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/GameSystem/AttackManager.h"
#include "../EnemyBase.h"
#include "../../Player/ToJson/PlayerAttackData.h"
#include "../State/EnemyDamageState.h"
#include "EnemyOnHit.h"

EnemyOnHit::EnemyOnHit(EnemyBase& _parent)
	: parent_(_parent)
	, cnt_(0.0f)
{
}

EnemyOnHit::~EnemyOnHit(void)
{
}

void EnemyOnHit::CalcDamage(const std::weak_ptr<Collider> _col)
{
	//攻撃マネージャー
	auto& atkMng = AttackManager::GetInstance();

	//ヒット者
	const auto& col = _col.lock();

	//自身のコライダ
	const auto& myCol = parent_.GetColliders()[0];

	//本体にあたってないので無視
	if (!myCol->IsHit())return;

	//当たれるか
	if (!atkMng.IsCanHit(col, myCol)) return;

	//攻撃情報
	const auto& atkData = atkMng.GetAttackData(_col, myCol).lock();
	auto data = dynamic_pointer_cast<AttackData>(atkData);

	//攻撃間隔
	if (data->hitInterval < cnt_ && data->isMultiHit)
	{
		//カウンタ
		auto& scnMng = SceneManager::GetInstance();
		cnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());
		return;
	}

	//ここからヒット処理

	//現在スキルの破棄
	parent_.RemoveCurrentSkill();

	//リセット
	cnt_ = 0.0f;

	//回転情報
	const Quaternion quaRot = col->GetGeometry().GetColRot();

	//吹っ飛び(相手の向いている方向)
	VECTOR blowPow = VScale(quaRot.GetForward(), BLOW_POWER);

	//吹っ飛びの移動量を与える
	parent_.SetMovePow(blowPow);

	//ダメージ状態
	parent_.ChangeState(std::make_unique<EnemyDamageState>());

	//ダメージ処理
	parent_.Damage(data->power);
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
