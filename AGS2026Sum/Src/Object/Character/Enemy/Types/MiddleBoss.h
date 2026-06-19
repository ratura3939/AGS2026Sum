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

	//初期化
	void DoInit(void)override;

	//描画
	void Draw(void)override;

	//ガードブレイク値リセット
	void ResetGuardBreakPoint(void) { guardBreakPoint_ = GUARD_BREAK_POINT_MAX; }

private:

	//ガードブレイク最大値
	static constexpr float GUARD_BREAK_POINT_MAX = 100.0f;

	//HPバーの位置
	static constexpr int HP_LOCAL_POS_X = -100;
	static constexpr int HP_LOCAL_POS_Y = -200;
	static constexpr int HP_WIDTH = 500;
	static constexpr int HP_HEIGHT =  20;
	static constexpr int HP_WINDOW = 5;

	//ガードブレイク値
	float guardBreakPoint_;
};

