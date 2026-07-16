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

			//ボスの生成座標
			VECTOR pos;
		};

		//スポーン地点
		struct SpawnArea
		{
			//中心地
			VECTOR center = {};

			//大きさ
			float size = 0.0f;
		};

		//グループ数
		int groupNum = 0;

		//敵の数
		int enemyNum = 0;

		//スポーン地点
		std::vector<SpawnArea> spawnAreas;

		//ボス情報
		std::vector<BossInfo> bossInfos = {};
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

		//グループ数
		stageJson.at("groupNum").get_to(stageInfo.groupNum);
		
		//敵の数
		stageJson.at("enemyNum").get_to(stageInfo.enemyNum);

		//ボス情報
		for (const auto& bossJson : stageJson["bossInfos"])
		{
			//ボス情報
			StageEnemyData::AllEnemysInfo::BossInfo bossInfo;

			//ボスタイプ
			bossInfo.bossType = CastStringToEnemyType(bossJson.at("type"));

			//イベント名
			bossInfo.eventType = CastStringToEventType(bossJson.at("eventKey"));

			//座標
			bossInfo.pos.x = bossJson.at("pos").at("x");
			bossInfo.pos.y = bossJson.at("pos").at("y");
			bossInfo.pos.z = bossJson.at("pos").at("z");

			//格納
			stageInfo.bossInfos.push_back(bossInfo);
		}

		//スポーン地点
		for (const auto& jsonSpawnArea : stageJson["spawnAreas"])
		{
			//スポーン地点
			StageEnemyData::AllEnemysInfo::SpawnArea spawnArea;

			//中心地
			spawnArea.center.x = jsonSpawnArea.at("center").at("x");
			spawnArea.center.y = jsonSpawnArea.at("center").at("y");
			spawnArea.center.z = jsonSpawnArea.at("center").at("z");
			
			//大きさ
			jsonSpawnArea.at("size").get_to(spawnArea.size);

			//格納
			stageInfo.spawnAreas.push_back(spawnArea);
		}

		//格納
		_param.allStageInfo.emplace(stageName, stageInfo);
	}
}