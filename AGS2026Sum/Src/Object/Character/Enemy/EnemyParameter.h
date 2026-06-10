#pragma once
#include"../../../Lib/nlohmann/json.hpp"
#include<vector>

struct EnemyParameter
{
	//体力
	int hp = 0;

	//モデル
	int modelId = -1;

	//アニメーション
	std::vector<int> anim;
};

inline void from_json(const nlohmann::json& _data, EnemyParameter& _attackData) 
{
	_data.at("radius").get_to(_attackData.radius);
	_data.at("power").get_to(_attackData.power);

	if (_data.contains("localPos")) {
		_data.value("localPos", nlohmann::json::object()).at("x").get_to(_attackData.localPos.x);
		_data.value("localPos", nlohmann::json::object()).at("y").get_to(_attackData.localPos.y);
		_data.value("localPos", nlohmann::json::object()).at("z").get_to(_attackData.localPos.z);
	}
	_data.at("animationSpeed").get_to(_attackData.animationSpeed);

	if (_data.contains("nextAttacks")) {
		const auto& next = _data.at("nextAttacks");
		for (int i = 0; i < ATTACK_TYPE_NUM; i++) {
			_attackData.nextAttacks[i] = next[i].get<std::string>();
		}
	}
}