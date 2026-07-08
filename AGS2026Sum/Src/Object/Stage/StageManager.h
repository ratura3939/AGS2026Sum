#pragma once
#include<memory>
#include "../Common/ActorBase.h"

class ModelRenderer;
class ModelMaterial;
class AutoDoor;

class StageManager :
    public ActorBase
{
public:
    StageManager(void);
    ~StageManager(void) override;
    void Draw(void) override;
	void Release(void) override;

	void HitCollider(std::weak_ptr<Collider> _col) override {};

	void OpenDoor(void);	//ドアを開く
	void CloseDoor(void);	//ドアを閉じる

private:
	void DoLoad(void) override;
	void DoInit(void) override;
	void DoUpdate(void) override;

	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	std::unique_ptr<AutoDoor> door_;	//ステージ仕切り用のドア
};

