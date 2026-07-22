#pragma once
#include"../../../../Lib/nlohmann/json.hpp"
#include<DxLib.h>
#include<vector>
#include<string>
#include<unordered_map>

//敵パラメーター
struct EnemyParameter
{
	//体力
	int initHp = -1;

	//モデル
	std::string modelName = "";

	//攻撃の種類
	std::vector<std::string> skillName;
};

inline void from_json(const nlohmann::json& _data, EnemyParameter& _param) 
{
	//体力
	_data.at("hp").get_to(_param.initHp);

	//モデル名
	_data.at("modelName").get_to(_param.modelName);

	//攻撃名
	if (_data.contains("skill"))
	{
		//スキル配列
		const auto& skillJson = _data.at("skill");

		for (const auto& skillName : skillJson)
		{
			_param.skillName.push_back(skillName);
		}
	}
}