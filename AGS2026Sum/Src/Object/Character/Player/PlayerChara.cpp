#include "../../../pch.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../../../Manager/Generic/ResourceManager.h"		
#include "PlayerChara.h"

namespace {
	const float MOVE_SPEED = 0.1f;	//移動速度
}

PlayerChara::PlayerChara(void)
{
}

PlayerChara::~PlayerChara(void)
{
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
}

void PlayerChara::DoUpdate(void)
{
	Move();
	Rotation();
	inputDir_ = Utility::VECTOR_ZERO;
}

void PlayerChara::Move(void)
{
	VECTOR moveVec = inputDir_;
	moveVec.y = 0.0f;	//Y軸方向の移動はなし

	pos_ = VAdd(pos_, VScale(moveVec, moveSpeed_));	//移動
}

void PlayerChara::Attack(void)
{
}

void PlayerChara::Draw(void)
{
	const VECTOR& cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	DrawFormatString(10, 30, 0xffffff, L"PlayerPos: %f, %f, %f,\nInputDir: %f, %f, %f\nCameraPos: %f, %f, %f", pos_.x, pos_.y, pos_.z, inputDir_.x, inputDir_.y, inputDir_.z, cameraPos.x, cameraPos.y, cameraPos.z);
	MV1DrawModel(modelId_);
	DrawSphere3D(pos_, 8, 8, 0xff0000, 0xff0000, false);
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
