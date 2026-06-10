#pragma once
#include "EnemyBase.h"

class MiddleBoss : public EnemyBase
{
private:

	//読み込み
	void DoLoad(void)override;

	//初期化
	void DoInit(void)override;

	//アニメーション初期化
	void InitAnim(void)override;
};

