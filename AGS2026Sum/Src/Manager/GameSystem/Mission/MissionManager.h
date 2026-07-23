#pragma once
#include<DxLib.h>
#include<array>
#include<memory>
#include "../Common/Singleton.h"
#include"../Object/Character/Enemy/Info/EnemyDefine.h"

class MissionBase;

class MissionManager : public Singleton<MissionManager>
{
	//シングルトン化のため、Singletonクラスをフレンドクラスに指定
	friend class Singleton<MissionManager>;

public:
	
	//更新
	void Update(void);
	
	//描画
	void Draw(void);

	//ミッションの設定
	void SetMission(std::unique_ptr<MissionBase> _mission);

	//進捗を進める
	void AddProgress(const ENEMY_TYPE& _enemy);

	//ミッションの達成判定
	const bool IsCompleted(void)const;

private:

	//ミッション
	std::unique_ptr<MissionBase> mission_;

	//コンストラクタ
	MissionManager(void);

	//デストラクタ
	~MissionManager(void)override;
};

