#include "../../../pch.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "NormalEnemy.h"

//親ボーン名
const std::wstring EnemyBase::ROOT_NAME = L"mixamorig:Hips";

void NormalEnemy::DoLoad(void)
{
	//モデル差し込み
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MDL);

	//アニメーションの初期化
	InitAnim();

	//共通
	EnemyBase::DoLoad();
}

void NormalEnemy::DoInit(void)
{
	//体力
	hp_ = 10.0f;
	quaRotLocal_ = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	//共通
	EnemyBase::DoInit();
}

void NormalEnemy::InitAnim(void)
{
	//インスタンス取得
	auto& res = ResourceManager::GetInstance();

	//アニメーションの初期化
	animController_ = std::make_unique<AnimationController>(modelId_);

	//待機アニメーション
	int animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_IDLE_ANIM);
	animController_->Add(L"Idle", animData, AnimationController::PLAY_TYPE::LOOP, AnimationController::ANIM_SOURCE::EXTERNAL);

	//歩きアニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_WALK_ANIM);
	animController_->Add(L"Walk", animData, AnimationController::PLAY_TYPE::LOOP, AnimationController::ANIM_SOURCE::EXTERNAL);

	//走りアニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_RUN_ANIM);
	animController_->Add(L"Run", animData, AnimationController::PLAY_TYPE::LOOP, AnimationController::ANIM_SOURCE::EXTERNAL);

	//攻撃アニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_ATTACK_ANIM);
	animController_->Add(L"Attack", animData, AnimationController::PLAY_TYPE::NORMAL, AnimationController::ANIM_SOURCE::EXTERNAL);

	//吹っ飛びアニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_BLOW_ANIM);
	animController_->Add(L"Blow", animData, AnimationController::PLAY_TYPE::NORMAL, AnimationController::ANIM_SOURCE::EXTERNAL, false, true);
	animController_->SetFixAnimationAxisInfo(L"Blow", true, true, true);

	//死亡アニメーション
	animData = res.LoadModelDuplicate(ResourceManager::SRC::ENEMY_DEATH_ANIM);
	animController_->Add(L"Death", animData, AnimationController::PLAY_TYPE::NORMAL, AnimationController::ANIM_SOURCE::EXTERNAL);

	//重心ボーンの設定
	animController_->SetRootFrameIndex(ROOT_NAME);

	//デフォルトアニメーションの設定
	animController_->SetDefaultAnim(L"Idle");
}
