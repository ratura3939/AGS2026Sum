#include "../../../../pch.h"
#include "../../../../Manager/Generic/ResourceManager.h"
#include "../../Attack/AttackDataBase.h"
#include "EnemySkillBase.h"
#include "EnemyNormalSkill.h"
#include "EnemyTackleSkill.h"
#include "EnemyJumpSkill.h"
#include "EnemySkillFactory.h"

EnemySkillFactory::EnemySkillFactory(void)
{
	//スキル生成
	createSkill_[L"NormalSkill"] = []()->std::unique_ptr<EnemySkillBase> {return std::make_unique<EnemyNormalSkill>();};
	createSkill_[L"Tackle"] = []()->std::unique_ptr<EnemySkillBase> {return std::make_unique<EnemyTackleSkill>();};
	createSkill_[L"Jump"] = []()->std::unique_ptr<EnemySkillBase> {return std::make_unique<EnemyJumpSkill>();};
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