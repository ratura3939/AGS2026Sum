#pragma once
#include<DxLib.h>
#include"../../../../Lib/nlohmann/json.hpp"

//jsonから受け取る攻撃データ
struct AttackDirectionData {
	float seTiming = -1.0f;		//効果音発生タイミング
	float efcTiming = -1.0f;	//エフェクト発生タイミング
	float efcScale = -1.0f;		//エフェクトサイズ
	float efcSpeed = -1.0f;		//エフェクト再生速度
	VECTOR efcLocalPos = { -1.0f, -1.0f, -1.0f };		//ローカル座標
	VECTOR efcLocalRot = { -1.0f, -1.0f, -1.0f };		//ローカル回転
};

inline void from_json(const nlohmann::json& _data, AttackDirectionData& _attackDirectionData) {
	_data.at("seTimingRate").get_to(_attackDirectionData.seTiming);
	_data.at("efcTimingRate").get_to(_attackDirectionData.efcTiming);
	_data.at("efcScale").get_to(_attackDirectionData.efcScale);
	_data.at("efcSpeed").get_to(_attackDirectionData.efcSpeed);

	if (_data.contains("efcLocalPos")) {
		_data.value("efcLocalPos", nlohmann::json::object()).at("x").get_to(_attackDirectionData.efcLocalPos.x);
		_data.value("efcLocalPos", nlohmann::json::object()).at("y").get_to(_attackDirectionData.efcLocalPos.y);
		_data.value("efcLocalPos", nlohmann::json::object()).at("z").get_to(_attackDirectionData.efcLocalPos.z);
	}

	if (_data.contains("efcLocalRot")) {
		_data.value("efcLocalRot", nlohmann::json::object()).at("x").get_to(_attackDirectionData.efcLocalRot.x);
		_data.value("efcLocalRot", nlohmann::json::object()).at("y").get_to(_attackDirectionData.efcLocalRot.y);
		_data.value("efcLocalRot", nlohmann::json::object()).at("z").get_to(_attackDirectionData.efcLocalRot.z);
	}
}