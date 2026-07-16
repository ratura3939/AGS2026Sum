#include "../../../../../pch.h"
#include "../../../../../Application.h"
#include "../../../../../Manager/Generic/SceneManager.h"
#include "../../../../../Manager/Generic/Camera.h"
#include"../../../../../Renderer/ModelMaterial.h"
#include"../../../../../Renderer/ModelRenderer.h"
#include "../../Info/EnemyDefine.h"
#include "../../State/EnemyNormalState.h"
#include "../../Brain/MiddleBossBrain.h"
#include "../../OnHit/BossOnHit.h"
#include "../../../../Common/Collider.h"
#include "../../../../Common/Geometry/Sphere.h"
#include "MiddleBoss.h"

MiddleBoss::MiddleBoss(void)
	: EnemyBase(ENEMY_TYPE::MIDDLE_BOSS)
	, battle_()
{
}

MiddleBoss::~MiddleBoss(void)
{
}

void MiddleBoss::DoLoad(void)
{
	//思考の初期化
	brain_ = std::make_unique<MiddleBossBrain>(*this);

	//接触処理の初期化
	onHit_ = std::make_unique<BossOnHit>(*this);

	//シェーダーの読み込み
	LoadShader();
}

void MiddleBoss::LoadShader(void)
{
	//共通
	EnemyBase::LoadShader();

	outlineMaterial_ = std::make_unique<ModelMaterial>(L"SkinOutLineVS.cso", 2, L"SkinOutLinePS.cso", 1);
	outlineMaterial_->AddConstBufVS(FLOAT4{ 2.0f,0.0f,0.0f,0.0f });

	const VECTOR& cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	outlineMaterial_->AddConstBufVS(FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });
	outlineMaterial_->AddConstBufPS(FLOAT4{ 0.0f,0.0f,0.0f,1.0f });
}

void MiddleBoss::DoInit(void)
{
	//共通
	EnemyBase::DoInit();

	//戦闘情報リセット
	battle_.Reset();
}

void MiddleBoss::DoUpdate(void)
{
	//戦闘情報更新
	if(!IsEndState() && !IsFade())battle_.Update();

	//共通更新
	EnemyBase::DoUpdate();
}

void MiddleBoss::Draw(void)
{
	//デバッグ描画
	DrawDebug();

	//戦闘情報描画
	battle_.Draw();

	//モデル描画
	//MV1DrawModel(modelId_);

	//カメラ座標
	const VECTOR& cameraPos = SceneManager::GetInstance().GetCamera().GetPos();

	//描画
	outlineMaterial_->SetConstBufVS(1, FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });

	//アウトライン用描画
	MV1SetWriteZBuffer(modelId_, false);					//モデル描画のZBufferを無効にする
	MV1SetMeshBackCulling(modelId_, 0, DX_CULLING_RIGHT);	//裏面描画
	modelRenderer_->Draw(modelId_,*outlineMaterial_);

	//本体描画
	MV1SetWriteZBuffer(modelId_, true);
	MV1SetMeshBackCulling(modelId_, 0, DX_CULLING_LEFT);	//表面描画

	//モデルの描画
	modelRenderer_->Draw(modelId_, *modelMaterial_);

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
		DrawBox(healthPos.x, healthPos.y, healthPos.x + HP_WIDTH * (hp_ / hpMax_), healthPos.y + HP_HEIGHT, 0xff0000, true);
	}
}

void MiddleBoss::CreateCollider(void)
{
	//コライダの初期化
	DeleteCollider();

	//当たり判定の生成(ボスはほかの敵と接触しない)
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(pos_, movedPos_, quaRot_, BROUD_RADIUS, RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY, { Collider::COL_TAG::PLAYER, Collider::COL_TAG::PLAYER_ATTACK,Collider::COL_TAG::STAGE });

	//攻撃コライダ
	geo = std::make_unique<Sphere>(attackPos_, attackPos_, quaRot_, ATTACK_BROUD_RADIUS, ATTACK_RADIUS);
	MakeCollider(std::move(geo), Collider::COL_TAG::ENEMY_ATTACK, { Collider::COL_TAG::PLAYER });
	DisableColliderAtTag(Collider::COL_TAG::ENEMY_ATTACK);
}