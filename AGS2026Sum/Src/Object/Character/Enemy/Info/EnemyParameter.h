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

	//メインボーン名
	//std::string mainFrameName = "";

	//アニメーション
	//std::unordered_map<std::string, AnimParameter> animParam;

	//攻撃の種類
	std::vector<std::string> skillName;
};

inline void from_json(const nlohmann::json& _data, EnemyParameter& _param) 
{
	//体力
	_data.at("hp").get_to(_param.initHp);

	//モデル名
	_data.at("modelName").get_to(_param.modelName);

	//メインボーン名
	//_data.at("mainFrameName").get_to(_param.mainFrameName);

	//アニメーション関係
	//if (_data.contains("animation")) {

	//	//アニメーション配列
	//	const auto& animationJson = _data.at("animation");

	//	//アニメーション分取得
	//	for (const auto& [animName, animJson] : animationJson.items())
	//	{
	//		//アニメーション情報
	//		EnemyParameter::AnimParameter animParam = {};

	//		//ループの有無
	//		animParam.isLoop = animJson.value("isLoop", false);

	//		//ロック
	//		animParam.isLock = animJson.value("isLock", false);

	//		//位置固定
	//		if (animJson.contains("fixPos"))
	//		{
	//			animParam.isFixPos = true;
	//			const auto& fixPos = animJson.at("fixPos");
	//			animParam.fixPos.x = fixPos.value("x", false);
	//			animParam.fixPos.y = fixPos.value("y", false);
	//			animParam.fixPos.z = fixPos.value("z", false);
	//		}

	//		//格納
	//		_param.animParam.emplace(animName,animParam);
	//	}
	//}

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