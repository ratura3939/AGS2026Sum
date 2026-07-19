#pragma once
#include<array>
#include<memory>
#include"Collider.h"

class ActorBase;
class Collider;

class OnHitBase
{
public:

	//コンストラクタ
	OnHitBase(void);

	//デストラクタ
	virtual~OnHitBase(void);

	//当たり判定
	virtual void HitCollider(const std::weak_ptr<Collider> _col);

protected:

	//親設定は各自で

	//当たり判定用の関数ポインタ
	using Func = void(OnHitBase::*)(const std::weak_ptr<Collider> _col);

	//当たり判定のタグごとの処理
	std::array<Func, static_cast<int>(Collider::COL_TAG::MAX)> onHit_;

	//当たり判定のタグごとの処理
	virtual void HitPlayer(const std::weak_ptr<Collider> _col) = 0;
	virtual void HitPlayerAttack(const std::weak_ptr<Collider> _col) = 0;
	virtual void HitEnemy(const std::weak_ptr<Collider> _col) = 0;
	virtual void HitEnemyAttack(const std::weak_ptr<Collider> _col) = 0;
	virtual void HitStage(const std::weak_ptr<Collider> _col) = 0;
};

