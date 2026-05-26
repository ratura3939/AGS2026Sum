#pragma once
#include<array>
#include"../../Common/Collider.h"

class EnemyBase;

class EnemyOnHit
{
public:

	//コンストラクタ
	EnemyOnHit(EnemyBase& _parent);
	
	//デストラクタ
	~EnemyOnHit(void);

	//当たり判定
	void HitCollider(const std::weak_ptr<Collider> _col);

private:

	//親
	EnemyBase& parent_;

	//当たり判定用の関数ポインタ
	using Func = void(EnemyOnHit::*)(const std::weak_ptr<Collider> _col);

	//当たり判定のタグごとの処理
	std::array<Func, static_cast<int>(Collider::COL_TAG::MAX)> onHit_;

	//当たり判定のタグごとの処理
	void HitPlayer(const std::weak_ptr<Collider> _col);
	void HitPlayerAttack(const std::weak_ptr<Collider> _col);
	void HitEnemy(const std::weak_ptr<Collider> _col);
	void HitEnemyAttack(const std::weak_ptr<Collider> _col);
};