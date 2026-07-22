#include "../../pch.h"
#include "EventManager.h"

void EventManager::AddFlagCount(const EVENT_TYPE& _event)
{
	flagCnt_[static_cast<int>(_event)]++;
}

void EventManager::SubFlagCount(const EVENT_TYPE& _event)
{
	flagCnt_[static_cast<int>(_event)]--;
}

const bool EventManager::IsPlayEvent(const EVENT_TYPE& _event)
{
	return 	flagCnt_[static_cast<int>(_event)] <= 0;
}

EventManager::EventManager(void)
{
	flagCnt_.fill(0);
}

EventManager::~EventManager(void)
{
}