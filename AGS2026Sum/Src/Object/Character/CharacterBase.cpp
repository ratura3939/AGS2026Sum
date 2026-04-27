#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Utility/Utility.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void)
	:ActorBase()
	,characterRotY_(Quaternion::Identity())
	,goalQua_(Quaternion::Identity())
	,stepRotation_(0.0f)
	,hp_(-1.0f)
	,animController_(nullptr)
{
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::SetGoalRot(const float _rad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera().GetAngle();

	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + _rad, Utility::AXIS_Y);
	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, characterRotY_);
	// しきい値より大きかったら
	if (angleDiff > THRESHOLD_ROT) {
		//回転し始めに設定
		stepRotation_ = 0.0f;
	}
	goalQua_ = axis;
}

void CharacterBase::Rotation(void)
{
	//回転時間増加
	stepRotation_ += PER_ROT;
	// 回転の球面補間
	characterRotY_ = Quaternion::Slerp(
		characterRotY_, goalQua_, stepRotation_);

	//初期化を行い新たな回転量を設定する
	quaRot_ = Quaternion();
	quaRot_ = quaRot_.Mult(characterRotY_);
}

void CharacterBase::Death(void)
{
	//死亡処理
}

void CharacterBase::Damage(const float _damage)
{
	hp_ -= _damage;
	if (!IsAlive()){
		Death();	//死亡処理
	}
}
