#include "../../../../pch.h"
#include "../../../../Application.h"
#include "../Info/EnemyDefine.h"
#include "../State/EnemyNormalState.h"
#include "../Brain/MiddleBossBrain.h"
#include "../OnHit/EnemyOnHit.h"
#include "../../../Common/Collider.h"
#include "../../../Common/Geometry/Sphere.h"
#include "MiddleBoss.h"

MiddleBoss::MiddleBoss(void)
	: EnemyBase(ENEMY_TYPE::MIDDLE_BOSS)
	, guardBreakPoint_(GUARD_BREAK_POINT_MAX)
{
}

MiddleBoss::~MiddleBoss(void)
{
}

void MiddleBoss::DoLoad(void)
{
	//状態の初期化
	state_ = std::make_unique<EnemyNormalState>();
	state_->Enter(*this);

	//思考の初期化
	brain_ = std::make_unique<MiddleBossBrain>(*this);

	//接触処理の初期化
	onHit_ = std::make_unique<EnemyOnHit>(*this);
}

void MiddleBoss::DoInit(void)
{
	//ローカル回転
	quaRotLocal_ = Quaternion();

	//コライダの初期化
	DeleteAllColliders();

	//当たり判定の生成(ボスはほかの敵と接触しない)
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, quaRot_, BROUD_RADIUS, RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY, { Collider::COL_TAG::PLAYER, Collider::COL_TAG::PLAYER_ATTACK });

	//攻撃コライダ
	geo = std::make_unique<Sphere>(attackPos_, attackPos_, quaRot_, ATTACK_BROUD_RADIUS, ATTACK_RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY_ATTACK, { Collider::COL_TAG::PLAYER });
	DisableColliderAtTag(Collider::COL_TAG::ENEMY_ATTACK);
}

void MiddleBoss::Draw(void)
{
	//デバッグ描画
	DrawDebug();

	//モデル描画
	MV1DrawModel(modelId_);

	//ボスUI
	Vector2 healthPos;
	Vector2 healthWH;
	healthPos.x = Application::SCREEN_SIZE_X / 2 + HP_LOCAL_POS_X;
	healthPos.y = Application::SCREEN_SIZE_Y / 2 + HP_LOCAL_POS_Y;
	healthWH.x = healthPos.x + HP_WIDTH;
	healthWH.y = healthPos.y + HP_HEIGHT;

	//HPバー
	DrawBox(healthPos.x - HP_WINDOW, healthPos.y - HP_WINDOW, healthWH.x + HP_WINDOW, healthWH.y + HP_WINDOW, 0x0, true);
	if (hp_ > 0.0f)
	{
		DrawBox(healthPos.x, healthPos.y, healthWH.x * (hp_ / hpMax_), healthWH.y, 0xff0000, true);
	}
}