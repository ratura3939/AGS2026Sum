#pragma once
#include<string>

//イベントの種類
enum class EVENT_TYPE
{
	NONE
	, OPEN_TUTORIAL_DOOR
	, OPEN_STAGE1_DOOR
	, GAME_CLEAR
	, MAX
};

//文字列をキャスト
static const EVENT_TYPE& CastStringToEventType(const std::string& _string)
{
	if (_string == "None") return EVENT_TYPE::NONE;
	else if (_string == "OpenTutorialDoor") return EVENT_TYPE::OPEN_TUTORIAL_DOOR;
	else if (_string == "OpenStage1Door") return EVENT_TYPE::OPEN_STAGE1_DOOR;
	else if (_string == "GameClear") return EVENT_TYPE::GAME_CLEAR;
	else return EVENT_TYPE::NONE;
}	