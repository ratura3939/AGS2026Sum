#pragma once
#include"EnemyOnHit.h"

class BossOnHit : public EnemyOnHit
{
public:

	//コンストラクタ
	BossOnHit(EnemyBase& _parent);

	//デストラクタ
	~BossOnHit(void)override;

private:

	//ダメージ
	void CalcDamage(const std::weak_ptr<Collider> _col) override;
};

