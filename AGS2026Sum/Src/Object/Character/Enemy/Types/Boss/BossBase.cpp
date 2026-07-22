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
#include "BossBase.h"

BossBase::BossBase(const ENEMY_TYPE& _type)
	: EnemyBase(_type)
	, battle_()
{
}

BossBase::~BossBase(void)
{
}

void BossBase::DoLoad(void)
{
	//思考の初期化
	brain_ = std::make_unique<MiddleBossBrain>(*this);

	//接触処理の初期化
	onHit_ = std::make_unique<BossOnHit>(*this);

	//シェーダーの読み込み
	LoadShader();
}

void BossBase::LoadShader(void)
{
	//共通
	EnemyBase::LoadShader();

	//アウトライン
	outlineMaterial_ = std::make_unique<ModelMaterial>(L"SkinOutLineVS.cso", 2, L"SkinOutLinePS.cso", 1);
	outlineMaterial_->AddConstBufVS(FLOAT4{ 2.0f,0.0f,0.0f,0.0f });
	const VECTOR& cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	outlineMaterial_->AddConstBufVS(FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });
	outlineMaterial_->AddConstBufPS(FLOAT4{ 0.0f,0.0f,0.0f,1.0f });

	//リムライト
	rimLightMaterial_ = std::make_unique<ModelMaterial>(L"SkinVS.cso", 0, L"RimLightPS.cso", 4);
	rimLightMaterial_->AddConstBufPS(FLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
	rimLightMaterial_->AddConstBufPS(FLOAT4{ 0.0f, 0.0f, 0.0f, 1.0f });
	rimLightMaterial_->AddConstBufPS(FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f});
	rimLightMaterial_->AddConstBufPS(FLOAT4{ 1.0f, 0.0f, 0.0f, 16.0f });

	//オーラ
	//auraMaterial_ = std::make_unique<ModelMaterial>(L"RimAuraVS.cso", 1, L"RimAuraPS.cso", 3);
	//float auraSize = 0.03f;
	//auraMaterial_->AddConstBufVS(FLOAT4{ auraSize ,1.0f,0.0f,0.0f });
	//auraMaterial_->AddConstBufPS(FLOAT4{ auraSize,1.0f,0.0f,0.0f });
	//auraMaterial_->AddConstBufPS(FLOAT4{ 1.0f,0.0f,0.0f,0.0f });
	//auraMaterial_->AddConstBufPS(FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });
}

void BossBase::DoInit(void)
{
	//共通
	EnemyBase::DoInit();

	//戦闘情報リセット
	battle_.Reset();
}

void BossBase::DoUpdate(void)
{
	//戦闘情報更新
	if (!IsEndState() && !IsFade())battle_.Update();

	//共通更新
	EnemyBase::DoUpdate();
}

void BossBase::Draw(void)
{
	//デバッグ描画
	//DrawDebug();

	//戦闘情報描画
	//battle_.Draw();

	//モデル描画
	//MV1DrawModel(modelId_);

	//カメラ座標
	const VECTOR& cameraPos = SceneManager::GetInstance().GetCamera().GetPos();

	//描画

	//アウトライン用描画
	MV1SetWriteZBuffer(modelId_, false);					//モデル描画のZBufferを無効にする
	MV1SetMeshBackCulling(modelId_, 0, DX_CULLING_RIGHT);	//裏面描画
	outlineMaterial_->SetConstBufVS(1, FLOAT4{ cameraPos.x, cameraPos.y, cameraPos.z, 0.0f });
	modelRenderer_->Draw(modelId_, *outlineMaterial_);

	//本体描画
	MV1SetWriteZBuffer(modelId_, true);
	MV1SetMeshBackCulling(modelId_, 0, DX_CULLING_LEFT);	//表面描画

	//リムライト描画
	rimLightMaterial_->SetConstBufPS(2, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,1.0f });
	modelRenderer_->Draw(modelId_, *rimLightMaterial_);

	//オーラ
	//static float cnt = 0.0f;
	//cnt += SceneManager::GetInstance().GetDeltaTime();
	//auraMaterial_->SetConstBufPS(0, FLOAT4{ 0.03f,1.0f,cnt,0.0f });
	//auraMaterial_->SetConstBufPS(2, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,0.0f });

	//モデルの描画
	//modelRenderer_->Draw(modelId_, *modelMaterial_);
}

void BossBase::CreateCollider(void)
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