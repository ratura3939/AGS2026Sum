#pragma once
#include"../Object/Character/Enemy/Info/EnemyDefine.h"

//ミッションの種類
enum class MISSION_TYPE
{
	NONE,
	GO_TO_GOAL,		//目的地到着
	WIPEOUT,		//全滅
	KILL_TARGET,	//特定の敵の撃破
	SURVIVE,		//生き延びる
	MAX
};

class MissionBase
{
public:

	//コンストラクタ
	MissionBase(void);

	//デストラクタ
	virtual ~MissionBase(void);

	//更新
	virtual void Update(void) = 0;

	//描画
	virtual void Draw(void) = 0;

	//ミッションの達成判定
	virtual const bool IsCompleted(void)const = 0;

	//進捗を増やす
	virtual void AddProgress(const ENEMY_TYPE& _enemyType) {}

	//説明を返す
	const std::wstring& GetExplanText(void)const { return explanText_; }
	const int GetExplanImg(void)const { return explanImg_; }

	//説明を設定
	void SetExplanText(const std::wstring& _text) { explanText_ = _text; }
	void SetExplanImg(const int _img) { explanImg_ = _img; }

protected:

	//ナビゲーションの座標
	static constexpr float NAVI_UV_U = 0.7f;
	static constexpr float NAVI_UV_V = 0.7f;

	//目的の説明(文字列または画像)
	std::wstring explanText_;
	int explanImg_;
};