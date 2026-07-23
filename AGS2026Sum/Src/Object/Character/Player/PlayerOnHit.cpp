#include "../../../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Decoration/EffectManager.h"
#include "../../../Manager/GameSystem/AttackManager.h"
#include"../../../Manager/GameSystem/ComboManager.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Common/Geometry/Model.h"
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

	//ヒットエフェクト
	auto& eff = EffectManager::GetInstance();
	//if (!eff.IsEffectPlay(parent_.GetSpeciesName(), EffectManager::EFFECT_NAME::PLAYER_HIT))
	eff.Play(parent_.GetSpeciesName(), EffectManager::EFFECT_NAME::PLAYER_HIT, parent_.GetPos(), parent_.GetQua(), 10.0f);

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

void PlayerOnHit::HitStage(const std::weak_ptr<Collider> _col)
{
	//各コライダ
	auto& mainCol = parent_.GetColliders()[0];

	//相手コライダ
	const auto& hitCol = _col.lock();

	//相手モデル
	const auto& model = hitCol->GetGeometry<Model>();
	const int hitNum = model->GetHitInfo().HitNum;

	//自身の球
	Sphere& mainSphere = dynamic_cast<Sphere&>(mainCol->GetGeometry());
	float radius = mainSphere.GetRadius();

	//移動後座標
	VECTOR pos = parent_.GetMovedPos();
	
	//法線の合計
	VECTOR totalNormal = Utility::VECTOR_ZERO;
	
	//最大深度
	float maxDepth = 0.0f;

	for (int i = 0; i < hitNum ; i++)
	{
		//当たったポリゴン
		const auto& dim = mainSphere.GetHitInfo().Dim[i];

		//当たった座標
		VECTOR hitPos = dim.HitPosition;

		//法線
		VECTOR normal = VNorm(dim.Normal);

		//深度
		float depth = radius - VDot(normal, VSub(pos, hitPos));

		//めり込んでるなら
		if (depth > 0.0f)
		{
			//加算
			totalNormal = VAdd(totalNormal, normal);

			//深さ更新
			maxDepth = std::max(maxDepth, depth);
		}
	}

	// 押し戻し
	if (maxDepth > 0.0f)
	{
		VECTOR N = VNorm(totalNormal);
		pos = VAdd(pos, VScale(N, maxDepth));
	}

	//位置設定
	parent_.SetMovedPos(pos);
}