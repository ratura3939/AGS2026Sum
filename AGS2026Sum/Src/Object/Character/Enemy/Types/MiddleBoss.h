#pragma once
#include "../EnemyBase.h"

class MiddleBoss : public EnemyBase
{
public:

	//コンストラクタ
	MiddleBoss(void);

	//デストラクタ
	~MiddleBoss(void)override;

	//読み込み
	void DoLoad(void)override;

	//ガードブレイク値リセット
	void ResetGuardBreakPoint(void) { guardBreakPoint_ = GUARD_BREAK_POINT_MAX; }

private:

	//ガードブレイク最大値
	static constexpr float GUARD_BREAK_POINT_MAX = 100.0f;

	//ガードブレイク値
	float guardBreakPoint_;
};

