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
};

