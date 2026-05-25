#pragma once
#include<DxLib.h>
#include"../../../Lib/nlohmann/json.hpp"

//jsonから受け取る攻撃データ
struct AttackData {
	int radius = -1;      //大きさ
	int power = -1;      //攻撃力
	int time = -1;       //時間
	VECTOR localPos = { -1.0f, -1.0f, -1.0f };   //ローカル座標
	float animationSpeed = -1.0f;	//アニメーションの再生速度
	int counter = 0;		//攻撃の時間管理
};

inline void from_json(const nlohmann::json& _data, VECTOR& _vec) {
	_data.at("x").get_to(_vec.x);
	_data.at("y").get_to(_vec.y);
	_data.at("z").get_to(_vec.z);
}

inline void from_json(const nlohmann::json& _data, AttackData& _attackData) {
	_data.at("radius").get_to(_attackData.radius);
	_data.at("power").get_to(_attackData.power);
	_data.at("time").get_to(_attackData.time);
	
	if (_data.contains("localPos")) {
		_data.value("localPos", nlohmann::json::object()).at("x").get_to(_attackData.localPos.x);
		_data.value("localPos", nlohmann::json::object()).at("y").get_to(_attackData.localPos.y);
		_data.value("localPos", nlohmann::json::object()).at("z").get_to(_attackData.localPos.z);
	}
	_data.at("animationSpeed").get_to(_attackData.animationSpeed);
}