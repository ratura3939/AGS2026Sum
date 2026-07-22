#pragma once
#include "../../Common/OnHitBase.h"

class PlayerChara;

class PlayerOnHit : public OnHitBase
{
public:

	//コンストラクタ
	PlayerOnHit(PlayerChara& _parent);

	//デストラクタ
	~PlayerOnHit(void)override;

private:

	//親
	PlayerChara& parent_;

	//カウンタ
	float cnt_;

	//ダメージ計算
	void CalcDamage(const std::weak_ptr<Collider> _col);

	//当たり判定のタグごとの処理
	void HitPlayer(const std::weak_ptr<Collider> _col)override;
	void HitPlayerAttack(const std::weak_ptr<Collider> _col)override;
	void HitEnemy(const std::weak_ptr<Collider> _col)override;
	void HitEnemyAttack(const std::weak_ptr<Collider> _col)override;
	void HitStage(const std::weak_ptr<Collider> _col)override;
};

