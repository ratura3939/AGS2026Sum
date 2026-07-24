#pragma once
#include <vector>
#include "MissionBase.h"

class MissionKillTarget : public MissionBase
{
public:

	//コンストラクタ
	MissionKillTarget(void);

	//デストラクタ
	~MissionKillTarget(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//達成判定
	const bool IsCompleted(void)const override;

	//目標タイプの設定
	void SetTargetType(const ENEMY_TYPE& _enemyType) { targetType_ = _enemyType; }

	//進捗を増やす
	void AddProgress(const ENEMY_TYPE& _enemyType)override;

	//目標数の設定
	void SetTargetNum(const int _targetNum) { targetNum_ = _targetNum; }

private:

	//進捗
	int progress_;

	//目標
	int targetNum_;

	//目標タイプ
	ENEMY_TYPE targetType_;
};