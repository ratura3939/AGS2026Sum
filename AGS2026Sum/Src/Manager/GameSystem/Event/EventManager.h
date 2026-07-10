#pragma once
#include <array>
#include "../../Common/Singleton.h"
#include "EventType.h"

class EventManager : public Singleton<EventManager>
{
	//シングルトン化のため、Singletonクラスをフレンドクラスに指定
	friend class Singleton<EventManager>;

public:

	//イベントフラグカウンタの増加
	void AddFlagCount(const EVENT_TYPE& _event);

	//イベントフラグカウンタの減少
	void SubFlagCount(const EVENT_TYPE& _event);

	//イベント開始できるか(フラグの持ち主がいなくなったら開始)
	const bool IsPlayEvent(const EVENT_TYPE& _event);

private:

	//イベントフラグ
	std::array<int, static_cast<int>(EVENT_TYPE::MAX)> flagCnt_;

	//コンストラクタ
	EventManager(void);

	//デストラクタ
	~EventManager(void)override;
};