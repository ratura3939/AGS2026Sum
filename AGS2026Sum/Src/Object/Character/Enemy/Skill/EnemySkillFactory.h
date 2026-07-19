#pragma once
#include<unordered_map>
#include"EnemySkillsData.h"

class EnemyManager;
class EnemySkillBase;

class EnemySkillFactory
{
public:

	//コンストラクタ
	EnemySkillFactory(void);

	//デストラクタ
	~EnemySkillFactory(void);

	//スキルの生成
	std::unique_ptr<EnemySkillBase> CreateSkill(std::wstring _skillName);

private:

	//生成用関数ポインタ
	using CreateFuncPtr = std::unique_ptr<EnemySkillBase>(*)();

	//生成関数
	std::unordered_map<std::wstring, CreateFuncPtr> createSkill_;
};

