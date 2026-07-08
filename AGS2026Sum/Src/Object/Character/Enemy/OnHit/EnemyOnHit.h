#pragma once
#include<unordered_map>
#include"../../../Common/OnHitBase.h"

class EnemyBase;
class EnemyStateBase;

class EnemyOnHit : public OnHitBase
{
public:

	//コンストラクタ
	EnemyOnHit(EnemyBase& _parent);

	//デストラクタ
	~EnemyOnHit(void)override;

protected:

	//親
	EnemyBase& parent_;

	//カウンタ
	float cnt_;

	//状態変化関数ポインタ
	using Func = std::unique_ptr<EnemyStateBase>(EnemyOnHit::*)(const VECTOR& _vec);
	std::unordered_map<std::string, Func> createState_;

	//ダメージ計算
	virtual void CalcDamage(const std::weak_ptr<Collider> _col);

	//当たり判定のタグごとの処理
	void HitPlayer(const std::weak_ptr<Collider> _col)override;
	void HitPlayerAttack(const std::weak_ptr<Collider> _col)override;
	void HitEnemy(const std::weak_ptr<Collider> _col)override;
	void HitEnemyAttack(const std::weak_ptr<Collider> _col)override;

	//状態生成関数
	std::unique_ptr<EnemyStateBase> CreateStagger(const VECTOR& _vec);
	std::unique_ptr<EnemyStateBase> CreateLaunch(const VECTOR& _vec);
	std::unique_ptr<EnemyStateBase> CreateSlam(const VECTOR& _vec);
	std::unique_ptr<EnemyStateBase> CreatePushBack(const VECTOR& _vec);
	std::unique_ptr<EnemyStateBase> CreateBlowAway(const VECTOR& _vec);
};