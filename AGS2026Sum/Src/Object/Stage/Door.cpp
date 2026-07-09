#include "../../pch.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Renderer/ModelMaterial.h"
#include"../../Renderer/ModelRenderer.h"
#include "Door.h"

namespace {
	const float SCALING = 2.5f;
	const VECTOR INIT_SCALE_FOR_TEST = { SCALING,SCALING,SCALING };

	const float INIT_DOOR_DIFF_X = 100.0f* SCALING;	//左ドアの初期位置差分X
	const float DOOR_MOVE_SPEED = 5.0f;		//ドアの移動速度

	const int VS_BUFF_NUM = 0;
	const int PS_BUFF_NUM = 0;
}

Door::Door(const VECTOR& _pos, DOOR_SIDE _side)
	:side_(_side)
	,state_(DOOR_STATE::CLOSE)
	,movePowerOfFrame_(0.0f)
	,isMoving_(false)
	,moveFunc_(&Door::MoveOpenDoor)
	,material_(std::make_unique<ModelMaterial>(L"StdModelVS.cso", VS_BUFF_NUM, L"StdModelPS.cso", PS_BUFF_NUM))
	,renderer_(std::make_unique<ModelRenderer>())
{
	pos_ = _pos;
	scl_ = INIT_SCALE_FOR_TEST;
}

Door::~Door(void)
{
}

void Door::Draw(void)
{
	renderer_->Draw(modelId_, *material_);
}

void Door::Release(void)
{
}

void Door::ChangeState(DOOR_STATE _state)
{
	state_ = _state;

	if (state_ == DOOR_STATE::MAX) {
		return;	//終了
	}

	//ドアの動作がないとき(OPEN,CLOSE)は、移動フラグをオフに
	if(state_==DOOR_STATE::OPEN || state_==DOOR_STATE::CLOSE) {
		isMoving_ = false;	//フラグオフ
		return;	//終了
	}

	//動きがある場合
	isMoving_ = true;	//フラグオン

	DoorMoveFunc useFunc = &Door::MoveOpenDoor;	//初期値オープン処理

	if (state_ == DOOR_STATE::CLOSING) {
		useFunc = &Door::MoveCloseDoor;			//クローズ処理に変更
	}

	moveFunc_ = useFunc;	//更新処理設定
}

void Door::DoLoad(void)
{
	ResourceManager::SRC loadSrc = ResourceManager::SRC::DOOR_RIGHT_MDL;	//初期値右モデル

	//左右でモデルの変更がある
	if(side_ == DOOR_SIDE::LEFT) {
		loadSrc = ResourceManager::SRC::DOOR_LEFT_MDL;	//左モデルに変更
	}

	//読み込み
	modelId_ = ResourceManager::GetInstance().LoadModelDuplicate(loadSrc);	

	quaRotLocal_ = Quaternion::Euler(0.0f, 0.0f, 0.0f);
}

void Door::DoInit(void)
{
	VECTOR positionRelative = VScale(GetRight(), INIT_DOOR_DIFF_X);	//初期値右差分

	//左右で初期位置の差分を変更
	if (side_ == DOOR_SIDE::LEFT)
	{
		positionRelative = VScale(GetLeft(), INIT_DOOR_DIFF_X);;		//左差分に変更
	}

	pos_ = VAdd(pos_, positionRelative);	//初期位置の設定
}

void Door::DoUpdate(void)
{
	//フレームごとの移動量を初期化
	movePowerOfFrame_ = 0;

	if (isMoving_) {
		(this->*moveFunc_)();
	}
}

//MEMO(7/9)
//現在ドアの移動はスローモーションの影響を受けない（基本カットシーン扱いのため）
//仕様でカットシーン扱いをしない場合、スローの影響を受ける方がよいのでそこらへん修正しよう

void Door::MoveOpenDoor(void)
{
	//移動量の保存
	movePowerOfFrame_ = DOOR_MOVE_SPEED;

	//開く時は自身の方向と同じに動く
	if (side_ == DOOR_SIDE::LEFT) {
		movedPos_ = VAdd(movedPos_, VScale(GetLeft(), movePowerOfFrame_));
	}
	else {
		movedPos_ = VAdd(movedPos_, VScale(GetRight(), movePowerOfFrame_));
	}
}

void Door::MoveCloseDoor(void)
{
	movePowerOfFrame_ = DOOR_MOVE_SPEED;

	//閉めるときは逆方向に動く
	if (side_ == DOOR_SIDE::LEFT) {
		movedPos_ = VAdd(movedPos_, VScale(GetRight(), movePowerOfFrame_));
	}
	else {
		movedPos_ = VAdd(movedPos_, VScale(GetLeft(), movePowerOfFrame_));
	}
}
