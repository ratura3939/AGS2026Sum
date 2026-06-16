#include "../../../../pch.h"
#include "EnemySkillBase.h"
#include "EnemyNormalSkill.h"
#include "EnemySkillFactory.h"

EnemySkillFactory::EnemySkillFactory(void)
{
	//スキル生成
	createSkill_ =
	{
		{L"NormalSkill", []()->std::unique_ptr<EnemySkillBase> {return std::make_unique<EnemyNormalSkill>();}}
	};
}

EnemySkillFactory::~EnemySkillFactory(void)
{
}

std::unique_ptr<EnemySkillBase> EnemySkillFactory::CreateSkill(std::wstring _skillName)
{
	//スキル名ごとの生成処理
	auto func = createSkill_.find(_skillName);

	//存在確認
	if (func != createSkill_.end())
	{
		//生成
		return func->second();
	}

	return nullptr;
}
