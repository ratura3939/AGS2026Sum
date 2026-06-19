#pragma once
#include "../SceneBase.h"

#include<vector>
#include<string>

class PauseScene :
    public SceneBase
{
public:
	PauseScene(void);
	~PauseScene(void)override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitUI(void);
	void InitSound(void)override;
	void InitEffect(void)override;

	//入力受付
	void InputUser(void);

	//MEMO:α②向けの突貫工事！	見直し必須

	void ResetUIDirectionParam(void);
	bool isSelectBackTitle_;	//タイトルに戻るか
};

