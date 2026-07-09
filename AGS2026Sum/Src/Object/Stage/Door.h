#pragma once
#include<memory>
#include "../Common/ActorBase.h"

class ModelRenderer;
class ModelMaterial;

class Door :
    public ActorBase
{
public:
    //どちらのドアか
    enum class DOOR_SIDE {
        LEFT
		,RIGHT
        ,MAX
    };

    //ドアの状態
    enum class DOOR_STATE {
        OPEN
        ,CLOSE
        ,OPENING
        ,CLOSING
        ,MAX
	};

	Door(const VECTOR& _pos, DOOR_SIDE _side);
    ~Door(void)override;

	void Draw(void)override;
	void Release(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override {};

	void ChangeState(DOOR_STATE _state);	//ドアの状態を変更する
	const DOOR_STATE GetState(void)const { return state_; }	//ドアの状態を取得

	const float GetMovePowerOfFrame(void)const { return movePowerOfFrame_; }	//ドアの移動量(フレームごと)を取得

private:
    void DoLoad(void)override;
    void DoInit(void)override;
    void DoUpdate(void)override;

	void MoveOpenDoor(void);	//ドアを開く
	void MoveCloseDoor(void);	//ドアを閉じる

	std::unique_ptr<ModelMaterial>material_;
	std::unique_ptr<ModelRenderer>renderer_;

    using DoorMoveFunc = void(Door::*)(void);
    DoorMoveFunc moveFunc_;	//ドアの移動関数

    DOOR_SIDE side_;		//ドアの左右
    DOOR_STATE state_;		//ドアの状態
	float movePowerOfFrame_;//ドアの移動量(フレームごと)
	bool isMoving_;			//ドアが動いているか
};

