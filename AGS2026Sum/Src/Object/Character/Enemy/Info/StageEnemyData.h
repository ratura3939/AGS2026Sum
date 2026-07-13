#pragma once
#include<DxLib.h>
#include<nlohmann/json.hpp>
#include<unordered_map>
#include<string>
#include"EnemyDefine.h"
#include "../../../../Manager/GameSystem/Event/EventType.h"

struct StageEnemyData
{
	//敵の全情報
	struct AllEnemysInfo
	{
		//ボス情報
		struct BossInfo
		{
			//ボスのタイプ
			ENEMY_TYPE bossType = ENEMY_TYPE::NORMAL;

			//ボスのイベント情報
			EVENT_TYPE eventType = EVENT_TYPE::NONE;
		};

		//グループ数
		int groupNum = 0;

		//敵の数
		int enemyNum = 0;

		//ボス数
		int bossGroupNum = 0;

		//生成座標
		VECTOR pos;

		//相対座標
		VECTOR localPos;

		//ボス情報
		BossInfo bossInfo = {};
	};

	//ステージ情報
	std::unordered_map<std::string, AllEnemysInfo> allStageInfo;
};

inline void from_json(const nlohmann::json& _data, StageEnemyData& _param)
{
	//モデルごとの情報
	for (const auto& [stageName, stageJson] : _data.items())
	{
		//ステージごとの情報
		StageEnemyData::AllEnemysInfo stageInfo = {};

		if (stageJson.contains("boss"))
		{
			//ボスタイプ
			stageInfo.bossInfo.bossType = CastStringToEnemyType(stageJson.at("boss").at("type"));

			//イベント名
			stageInfo.bossInfo.eventType = CastStringToEventType(stageJson.at("boss").at("eventKey"));
		}

		//グループ数
		stageJson.at("groupNum").get_to(stageInfo.groupNum);
		
		//敵の数
		stageJson.at("enemyNum").get_to(stageInfo.enemyNum);

		//ボス数
		stageJson.at("bossGroupNum").get_to(stageInfo.bossGroupNum);

		//座標
		stageInfo.pos.x = stageJson.at("pos").at("x");
		stageInfo.pos.y = stageJson.at("pos").at("y");
		stageInfo.pos.z = stageJson.at("pos").at("z");

		//相対座標
		stageInfo.localPos.x = stageJson.at("localPos").at("x");
		stageInfo.localPos.y = stageJson.at("localPos").at("y");
		stageInfo.localPos.z = stageJson.at("localPos").at("z");

		//格納
		_param.allStageInfo.emplace(stageName, stageInfo);
	}
}