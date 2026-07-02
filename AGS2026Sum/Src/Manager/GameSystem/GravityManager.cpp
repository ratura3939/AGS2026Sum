#include "../../pch.h"
#include"../Utility/Utility.h"
#include "GravityManager.h"

void GravityManager::Init(void)
{
	slerpPow_ = 0.0f;
}

void GravityManager::CalcGravity(const VECTOR& _dir, VECTOR& _pow, const float _per)
{
	// 重力方向
	VECTOR dirGravity = _dir;

	// 重力の強さ
	float gravityPow = GRAVITY_POW * _per;

	// 重力を作る
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//_pow に重力計算を行う(加速度)
	_pow = VAdd(_pow, gravity);

	//回転させる
	Calculate();

	// 内積
	float dot = VDot(dirGravity, _pow);
	if (dot >= 0.0f)
	{
		//重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		_pow = gravity;
	}
}

void GravityManager::Calculate(void)
{
	//重力方向
	VECTOR dirGravity = Utility::DIR_D;

	//重力の逆方向
	VECTOR dirUpGravity = VScale(dirGravity, -1.0f);

	// 現在の上方向(つまり、重力の反対方向)
	VECTOR up = quaRot_.GetUp();

	// ２つのベクトル間の回転量(差)を求める
	Quaternion rot = Quaternion::FromToRotation(up, dirUpGravity);

	// 求めた回転量で、現在の重力制御を回転させる(差が埋まる)
	quaRot_ = Quaternion::Slerp(quaRot_, rot.Mult(quaRot_), slerpPow_);
}

GravityManager::GravityManager(void)
{
	slerpPow_ = 0.0f;
}

GravityManager::~GravityManager(void)
{
}
