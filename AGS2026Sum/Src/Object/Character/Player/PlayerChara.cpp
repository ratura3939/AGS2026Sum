#include "../../../pch.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Generic/ResourceManager.h"	
#include"PlayerManager.h"
#include "PlayerChara.h"

namespace {
	const float MOVE_SPEED = 10.1f;	//移動速度
	const std::wstring ROOT_NAME = L"mixamorig8:Hips";
}

PlayerChara::PlayerChara(void)
	:inputDir_(Utility::VECTOR_ZERO)
	,isMove_(false)
	,isAttack_(false)
	,moveSpeed_(MOVE_SPEED)
	,afterMoveRad_(0.0f)
{
}

PlayerChara::~PlayerChara(void)
{
}

void PlayerChara::PlayAnim(const std::wstring& _animName, const float _speed)
{
	animController_->Play(_animName, _speed);	//アニメーションの新規再生
}

void PlayerChara::ForcePlayAnim(const std::wstring& _animName, const float _speed)
{
	animController_->ForcePlay(_animName, _speed);
}

void PlayerChara::GetAnimTotalTime(const std::wstring& _animName) const
{
	animController_->GetAnimTotalTime(_animName);	//アニメーションの総再生時間を取得
}

const float PlayerChara::GetCurrentAnimationProgressRate(void) const
{
	return animController_->GetCurrentAnimationProgressRate();
}

void PlayerChara::DrawNextAnimations(void)
{
	animController_->DrawNextAnimations();
}

void PlayerChara::DoLoad(void)
{
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER_MDL);	//モデル取得
}

void PlayerChara::DoInit(void)
{
	pos_ = Utility::VECTOR_ZERO;
	moveSpeed_ = MOVE_SPEED;
	inputDir_ = Utility::VECTOR_ZERO;

	InitAnim();	//アニメーションの初期化
}

void PlayerChara::DoUpdate(void)
{
	//アニメーション設定
	useAnim_ = PlayerManager::ANIM_IDLE;	//初期値は待機
	if (isMove_) {
		useAnim_ = PlayerManager::ANIM_RUN;	//走りアニメ
	}

	//攻撃中ではないとき
	if (!isAttack_) {
		Move();	//移動処理
		animController_->Play(useAnim_);	//アニメーションの新規再生
	}
	Rotation();
	inputDir_ = Utility::VECTOR_ZERO;
	animController_->Update();
}

void PlayerChara::InitAnim(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	using SRC = ResourceManager::SRC;
	using ANIM_PLAY_TYPE = AnimationController::PLAY_TYPE;
	using ANIM_SOURCE = AnimationController::ANIM_SOURCE;

	animController_ = std::make_unique<AnimationController>(modelId_);

	//アニメーションの登録
	animController_->Add(PlayerManager::ANIM_IDLE, resM.Load(SRC::PLAYER_IDLE_ANIM).handleId_, ANIM_PLAY_TYPE::LOOP, ANIM_SOURCE::EXTERNAL);
	animController_->Add(PlayerManager::ANIM_RUN, resM.Load(SRC::PLAYER_RUN_ANIM).handleId_, ANIM_PLAY_TYPE::LOOP, ANIM_SOURCE::EXTERNAL);
	animController_->Add(PlayerManager::ANIM_DAMAGE, resM.Load(SRC::PLAYER_DAMAGE_ANIM).handleId_, ANIM_PLAY_TYPE::NORMAL, ANIM_SOURCE::EXTERNAL);
	animController_->Add(PlayerManager::ANIM_FIRST_PUNCH, resM.Load(SRC::PLAYER_FIRST_PUNCH_ANIM).handleId_, ANIM_PLAY_TYPE::NORMAL, ANIM_SOURCE::EXTERNAL, true);
	animController_->Add(PlayerManager::ANIM_SECOND_PUNCH, resM.Load(SRC::PLAYER_SECOND_PUNCH_ANIM).handleId_, ANIM_PLAY_TYPE::NORMAL, ANIM_SOURCE::EXTERNAL, true);
	animController_->Add(PlayerManager::ANIM_THIRD_PUNCH, resM.Load(SRC::PLAYER_THIRD_PUNCH_ANIM).handleId_, ANIM_PLAY_TYPE::NORMAL, ANIM_SOURCE::EXTERNAL, true);
	animController_->Add(PlayerManager::ANIM_MIDDLE_KICK, resM.Load(SRC::PLAYER_MIDDLE_KICK_ANIM).handleId_, ANIM_PLAY_TYPE::NORMAL, ANIM_SOURCE::EXTERNAL, true);
	animController_->Add(PlayerManager::ANIM_HIGH_KICK, resM.Load(SRC::PLAYER_HIGH_KICK_ANIM).handleId_, ANIM_PLAY_TYPE::NORMAL, ANIM_SOURCE::EXTERNAL, true);
	animController_->Add(PlayerManager::ANIM_FINSH_KICK, resM.Load(SRC::PLAYER_FINISH_KICK_ANIM).handleId_, ANIM_PLAY_TYPE::NORMAL, ANIM_SOURCE::EXTERNAL, true, true);
	animController_->SetFixAnimationAxisInfo(PlayerManager::ANIM_FINSH_KICK, true, false, true);	//竜巻旋風脚の移動補正

	animController_->SetRootFrameIndex(ROOT_NAME);	//親ボーン名

	animController_->SetDefaultAnim(PlayerManager::ANIM_IDLE);	//デフォルトを待機にする
	animController_->Play(PlayerManager::ANIM_IDLE);			//待機アニメ再生
}

void PlayerChara::Move(void)
{
	VECTOR moveVec = inputDir_;
	moveVec.y = 0.0f;	//Y軸方向の移動はなし

	pos_ = VAdd(pos_, VScale(moveVec, moveSpeed_));	//移動
	isMove_ = false;
}

void PlayerChara::Attack(void)
{
	isAttack_ = true;
}

void PlayerChara::Draw(void)
{
	const VECTOR& cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	DrawFormatString(10, 30, 0xffffff, L"PlayerPos: %f, %f, %f,\nInputDir: %f, %f, %f\nCameraPos: %f, %f, %f", pos_.x, pos_.y, pos_.z, inputDir_.x, inputDir_.y, inputDir_.z, cameraPos.x, cameraPos.y, cameraPos.z);
	MV1DrawModel(modelId_);
	//DrawSphere3D(pos_, 8, 8, 0xff0000, 0xff0000, false);
}

void PlayerChara::Release(void)
{
}

void PlayerChara::HitCollider(std::weak_ptr<Collider> _col)
{
}

void PlayerChara::InputMoveVec(const VECTOR& _inputVec)
{

#pragma region 入力→移動方向
	// 入力がほぼゼロの場合
	if (abs(_inputVec.x) < 0.01f && abs(_inputVec.y) < 0.01f) {
		return;		//処理の必要なし
	}

	isMove_ = true;	//移動している
	auto& camera = SceneManager::GetInstance().GetCamera();
	
	VECTOR camForward = camera.GetRot().GetForward();	//カメラ前方向（Y軸方向を無視）
	camForward.y = 0.0f;
	camForward = VNorm(camForward);

	VECTOR camRight = camera.GetRot().GetRight();		//カメラ右方向（Y軸方向を無視）
	camRight.y = 0.0f;
	camRight = VNorm(camRight);

	inputDir_ = VAdd(									//入力方向をワールド座標に変換
		VScale(camForward, _inputVec.y),				//y: 前後（前が正）、x: 左右（右が正）
		VScale(camRight, _inputVec.x)
	);

	inputDir_ = VNorm(inputDir_);	//正規化
#pragma endregion

#pragma region モデルの角度設定
	afterMoveRad_ = 0.0f;
	afterMoveRad_ = atan2f(_inputVec.x, _inputVec.y);	//角度設定（モデルの角度は目標として設定する時にカメラを考慮するので、ここでは入力のみをみる）
	SetGoalRot(afterMoveRad_);	//回転目標角度を設定
#pragma endregion
}
