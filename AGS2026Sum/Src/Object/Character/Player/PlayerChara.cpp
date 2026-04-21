#include "../../../pch.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include "PlayerChara.h"

PlayerChara::PlayerChara(void)
{
}

PlayerChara::~PlayerChara(void)
{
}

void PlayerChara::DoInit(void)
{
	pos_ = Utility::VECTOR_ZERO;
}

void PlayerChara::DoUpdate(void)
{
	Rotation();
}

void PlayerChara::Draw(void)
{
}

void PlayerChara::Release(void)
{
}

void PlayerChara::HitCollider(std::weak_ptr<Collider> _col)
{
}

void PlayerChara::InputMoveVec(const VECTOR& _inputVec)
{
#pragma region 入力による移動方向の設定
	// 入力がほぼゼロの場合
	if (abs(_inputVec.x) < 0.01f && abs(_inputVec.y) < 0.01f) {
		inputDir_ = Utility::VECTOR_ZERO;
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
#pragma endregion
}
