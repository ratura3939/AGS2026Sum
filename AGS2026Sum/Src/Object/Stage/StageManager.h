#pragma once
#include<memory>
#include "../Common/ActorBase.h"

class ModelRenderer;
class ModelMaterial;
class StageObjectBase;
class AutoDoor;

class StageManager 
{
public:
    StageManager(void);
    ~StageManager(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void OpenDoor(void);	//ドアを開く
	void CloseDoor(void);	//ドアを閉じる

private:

	std::unique_ptr<StageObjectBase> object_;	//ステージオブジェクト
	std::unique_ptr<AutoDoor> door_;			//ステージ仕切り用のドア
};

