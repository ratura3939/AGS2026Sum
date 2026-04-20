#include"../../pch.h"
#include"../../Utility/Utility.h"
#include "../../../Common/Quaternion.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cube.h"
#include"Line.h"
#include"Model.h"

//***************************************************
//モデル
//***************************************************

Model::Model(const VECTOR& _pos, const VECTOR& _movedPos, const Quaternion& _rot, const float _broudRadius, const int _modelId)
	: Geometry(_pos, _movedPos, _rot, _broudRadius), parentModelId_(_modelId)
{
	hitLineInfo_ = {};
	std::memset(&hitInfo_, 0, sizeof(hitInfo_)); 
	hitResult_ = {};
}

Model::Model(const Model& _copyBase)
	: Geometry(_copyBase.GetColPos(), _copyBase.GetColMovedPos(), _copyBase.GetColRot(), _copyBase.GetBroudRadius())
{
	parentModelId_ = _copyBase.GetParentModel();
	hitLineInfo_ = {};
	std::memset(&hitInfo_, 0, sizeof(hitInfo_)); 
	hitResult_ = {};
}

Model::~Model(void)
{
	HitAfter();
}

void Model::Draw(const int _color)
{
}

const bool Model::IsHit(Geometry& _geometry)
{
	return _geometry.IsHit(*this);
}

const bool Model::IsHit(Model& _model)
{
	return false;
}

const bool Model::IsHit(Cube& _cube)
{
	return false;
}

const bool Model::IsHit(Sphere& _sphere)
{
	//判定
	auto col1 = MV1CollCheck_Sphere(GetParentModel(), -1, _sphere.GetColPos(), _sphere.GetRadius());
	auto col2 = MV1CollCheck_Sphere(GetParentModel(), -1, _sphere.GetColPos(), _sphere.GetRadius());

	//当たったか
	bool ret = col1.HitNum >= 1;

	//当たっていたら情報更新
	if (ret)
	{
		_sphere.SetHitInfo(col1);
		SetHitInfo(col2);
	}

	return ret;
}

const bool Model::IsHit(Capsule& _capsule)
{
	//判定
	auto col1 = MV1CollCheck_Capsule(GetParentModel(), -1, _capsule.GetPosTop(), _capsule.GetPosDown(), _capsule.GetRadius());
	auto col2 = MV1CollCheck_Capsule(GetParentModel(), -1, _capsule.GetPosTop(), _capsule.GetPosDown(), _capsule.GetRadius());

	//当たったか
	bool ret = col1.HitNum >= 1;

	//当たっていたら情報更新
	if (ret)
	{
		_capsule.SetHitInfo(col1);
		SetHitInfo(col2);
	}

	return ret;
}

const bool Model::IsHit(Line& _line)
{
	//判定
	auto col = MV1CollCheck_Line(GetParentModel(), -1, _line.GetPosPoint1(), _line.GetPosPoint2());

	//当たっていたら情報更新
	if (col.HitFlag)
	{
		_line.SetHitInfo(col);
		SetHitLineInfo(col);
	}
	return col.HitFlag;
}

void Model::HitAfter(void)
{
	if (hitInfo_.HitNum > 0 && hitInfo_.Dim != nullptr) 
	{
		//当たり判定情報の解放(球、カプセル)
		MV1CollResultPolyDimTerminate(hitInfo_);

		//再初期化
		std::memset(&hitInfo_, 0, sizeof(hitInfo_));
	}
}
