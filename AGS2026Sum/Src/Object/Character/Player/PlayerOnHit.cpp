#include "../../../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/ComboManager.h"
#include "../Attack/AttackDataBase.h"
#include "PlayerChara.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(PlayerChara& _parent)
	: parent_(_parent)
	,cnt_(0.0f)
{
}

PlayerOnHit::~PlayerOnHit(void)
{
}

void PlayerOnHit::CalcDamage(const std::weak_ptr<Collider> _col)
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

	//攻撃間隔
	if (atkData->hitInterval > cnt_ && atkData->isMultiHit)
	{
		//カウンタ
		auto& scnMng = SceneManager::GetInstance();
		cnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());
		return;
	}

	//ここからヒット処理

	//リセット
	cnt_ = 0.0f;

	//コンボリセット
	ComboManager::GetInstance().ResetCombo();

	//ダメージ処理
	parent_.Damage(atkData->power);
}

void PlayerOnHit::HitPlayer(const std::weak_ptr<Collider> _col)
{
}

void PlayerOnHit::HitPlayerAttack(const std::weak_ptr<Collider> _col)
{
}

void PlayerOnHit::HitEnemy(const std::weak_ptr<Collider> _col)
{
}

void PlayerOnHit::HitEnemyAttack(const std::weak_ptr<Collider> _col)
{
	//ダメージ処理
	CalcDamage(_col);
}
