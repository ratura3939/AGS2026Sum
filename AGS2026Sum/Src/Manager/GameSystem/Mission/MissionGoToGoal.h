#pragma once
#include<DxLib.h>
#include "MissionBase.h"

class MissionGoToGoal : public MissionBase
{
public:

	//コンストラクタ
	MissionGoToGoal(void);

	//デストラクタ
	~MissionGoToGoal(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//達成判定
	const bool IsCompleted(void)const override;

	//プレイヤー座標の設定
	void SetPlayerPos(const VECTOR* _playerPos) { playerPos_ = _playerPos; }

	//目的地設定
	void SetTargetPos(const VECTOR& _targetPos) { targetPos_ = _targetPos; }

	//許容する目的地の距離設定
	void SetArrivalDistance(const float _arrivalDistance) { arrivalDistance_ = _arrivalDistance; }

private:

	//指標の座標(参照)
	const VECTOR* playerPos_;

	//距離
	float distance_;

	//ターゲットの位置
	VECTOR targetPos_;
	float arrivalDistance_;
};

