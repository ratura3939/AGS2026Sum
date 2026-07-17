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
	static constexpr int STAGE_NUM = 2;		//ステージ総数

    StageManager(void);
    ~StageManager(void);

	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void OpenDoor(void);	//ドアを開く
	void CloseDoor(void);	//ドアを閉じる

	const VECTOR& GetGoalPosAtDoorOpen(void)const;	//ドアオープン時のカメラ目標位置
	const VECTOR& GetDoorPos(void)const;	//ドア位置

private:
	void UpdateTutorialStage(void);
	void UpdateFirstStage(void);

	void DrawTutorialStage(void);
	void DrawFirstStage(void);

	using Func = void(StageManager::*)(void);

	Func updateFunc_[STAGE_NUM];
	Func drawFunc_[STAGE_NUM];

	int progress_;
	std::unique_ptr<StageObjectBase>firstStage_;	//ステージオブジェクト
	std::unique_ptr<StageObjectBase>bossStage_;	//ボスステージオブジェクト
	std::unique_ptr<AutoDoor> door_;			//ステージ仕切り用のドア
};

