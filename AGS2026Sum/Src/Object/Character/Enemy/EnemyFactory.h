#pragma once
#include<memory>
#include"../../../Manager/Generic/ResourceManager.h"
#include"Info/EnemyAnimationData.h"
#include"Info/EnemyDefine.h"
#include"Info/EnemyParameter.h"
#include"Skill/EnemySkillFactory.h"

class EnemyBase;

class EnemyFactory
{
public:

	//コンストラクタ
	EnemyFactory(void);

	//デストラクタ
	~EnemyFactory(void);

	//外部ロード
	void Load(void);

	//新しい敵の生成
	std::unique_ptr<EnemyBase> CreateNewEnemy(const ENEMY_TYPE& _type);

	//敵の情報取得
	const EnemyParameter& GetParam(const ENEMY_TYPE& _type)const;

private:

	//スキルファクトリー
	EnemySkillFactory skillFactory;

	//敵のパラメータ情報
	std::array<EnemyParameter, static_cast<int>(ENEMY_TYPE::MAX)> parameters_;
	
	//敵のアニメーション情報
	EnemyAnimationData animDatas_;

	//敵生成用関数ポインタ
	using CreateFunc = std::unique_ptr<EnemyBase>(*)(void);

	//敵生成
	std::array<CreateFunc, static_cast<int>(ENEMY_TYPE::MAX)>create_;

	//モデル等のロード
	void LoadModelAndAnimation(EnemyBase& _enemy, const ENEMY_TYPE& _type);

	//スキルの生成
	void CreateSkill(EnemyBase& _enemy, const ENEMY_TYPE& _type);
};