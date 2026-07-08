#pragma once

class BossBattleComponent
{
public:

	//コンストラクタ
	BossBattleComponent(void);

	//デストラクタ
	~BossBattleComponent(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//リセット
	void Reset(void);

	//ガードブレイク
	void GuardBreak(const float _value);

	//ガードブレイクしたか
	const bool IsGuardBreak(void)const { return guardDurability_ <= 0.0f; }

	//ブレイクが終わったか
	const bool IsBreakEnd(void)const { return breakTime_ >= BREAK_TIME; }

private:

	//ガード耐久値最大値
	static constexpr float GUARD_DURABILITY_MAX = 100.0f;

	//ブレイク秒数
	static constexpr float BREAK_TIME = 10.0f;

	//ガード耐久値
	float guardDurability_;

	//ブレイク時間
	float breakTime_;
};