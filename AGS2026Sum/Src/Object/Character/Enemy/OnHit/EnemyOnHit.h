#pragma once
#include"../../../Common/OnHitBase.h"

class EnemyBase;

class EnemyOnHit : public OnHitBase
{
public:

	//コンストラクタ
	EnemyOnHit(EnemyBase& _parent);

	//デストラクタ
	~EnemyOnHit(void)override;

private:

	//吹っ飛び速度
	static constexpr float BLOW_POWER = 5.0f;

	//親
	EnemyBase& parent_;

	//カウンタ
	float cnt_;

	//ダメージ計算
	void CalcDamage(const std::weak_ptr<Collider> _col);

	//当たり判定のタグごとの処理
	void HitPlayer(const std::weak_ptr<Collider> _col)override;
	void HitPlayerAttack(const std::weak_ptr<Collider> _col)override;
	void HitEnemy(const std::weak_ptr<Collider> _col)override;
	void HitEnemyAttack(const std::weak_ptr<Collider> _col)override;
};