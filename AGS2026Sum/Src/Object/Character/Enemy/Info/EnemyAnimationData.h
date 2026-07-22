#pragma once
#include<nlohmann/json.hpp>
#include<unordered_map>
#include<string>

struct EnemyAnimationData
{
	//モデル一体のアニメーション情報
	struct AnimationInfo
	{
		//敵の個々のアニメーション情報
		struct AnimParameter
		{
			//座標固定
			struct FixPos
			{
				bool x = false;
				bool y = false;
				bool z = false;
			};

			//アニメーションループ
			bool isLoop = false;

			//アニメーションロック
			bool isLock = false;

			//座標固定の有無
			bool isFixPos = false;

			//座標固定
			FixPos fixPos = {};
		};

		//モデル名
		std::string mainFrameName = "";

		//アニメーション
		std::unordered_map<std::string, AnimParameter> animParam;
	};

	//敵一体の情報
	std::unordered_map<std::string, AnimationInfo> info;
};

inline void from_json(const nlohmann::json& _data, EnemyAnimationData& _param)
{
	//モデルごとの情報
	for (const auto& [modelName, modelJson] : _data.items())
	{
		//モデル一体の情報
		EnemyAnimationData::AnimationInfo info;

		//メインボーン名
		modelJson.at("mainFrameName").get_to(info.mainFrameName);

		//アニメーション分取得
		for (const auto& [animName, animJson] : modelJson.at("Animation").items())
		{
			//アニメーション情報
			EnemyAnimationData::AnimationInfo::AnimParameter animParam = {};

			//ループの有無
			animParam.isLoop = animJson.value("isLoop", false);

			//ロック
			animParam.isLock = animJson.value("isLock", false);

			//位置固定
			if (animJson.contains("fixPos"))
			{
				animParam.isFixPos = true;
				const auto& fixPos = animJson.at("fixPos");
				animParam.fixPos.x = fixPos.value("x", false);
				animParam.fixPos.y = fixPos.value("y", false);
				animParam.fixPos.z = fixPos.value("z", false);
			}

			//格納
			info.animParam.emplace(animName, animParam);
		}

		//格納
		_param.info.emplace(modelName, info);
	}
}