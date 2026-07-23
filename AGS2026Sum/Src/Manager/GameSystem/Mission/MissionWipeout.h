#pragma once
#include "MissionBase.h"

class MissionWipeout : public MissionBase
{
public:

	//コンストラクタ
	MissionWipeout(void);

	//デストラクタ
	~MissionWipeout(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//達成判定
	const bool IsCompleted(void)const override;

	//進捗を増やす
	void AddProgress(const ENEMY_TYPE& _enemyType)override;

	//目標数の設定
	void SetTargetNum(const int _targetNum) { targetNum_ = _targetNum; }

private:

	//進捗
	int progress_;

	//目標
	int targetNum_;
};

