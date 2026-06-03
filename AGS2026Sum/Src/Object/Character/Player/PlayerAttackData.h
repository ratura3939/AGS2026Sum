#pragma once
#include<DxLib.h>
#include"../../../Lib/nlohmann/json.hpp"

static const int ATTACK_TYPE_NUM = 2;	//攻撃の種類の数

//jsonから受け取る攻撃データ
struct AttackData {
	float radius = -1.0f;      //大きさ
	float power = -1.0f;      //攻撃力
	VECTOR localPos = { -1.0f, -1.0f, -1.0f };   //ローカル座標
	float animationSpeed = -1.0f;	//アニメーションの再生速度
	std::array<std::string, ATTACK_TYPE_NUM>nextAttacks;	//次の攻撃(パンチorキックの最大２派生)
};

inline void from_json(const nlohmann::json& _data, VECTOR& _vec) {
	_data.at("x").get_to(_vec.x);
	_data.at("y").get_to(_vec.y);
	_data.at("z").get_to(_vec.z);
}

inline void from_json(const nlohmann::json& _data, AttackData& _attackData) {
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