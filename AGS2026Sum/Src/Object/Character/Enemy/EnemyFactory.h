#pragma once
#include<memory>
#include"../../../Manager/Generic/ResourceManager.h"
#include"EnemyDefine.h"
#include"EnemyParameter.h"

class EnemyBase;

class EnemyFactory
{
public:

	//外部ロード
	void Load(void);

	//新しい敵の生成
	std::unique_ptr<EnemyBase> CreateNewEnemy(const ENEMY_TYPE& _type);

	//パラメーター取得
	const EnemyParameter& GetParam(const ENEMY_TYPE& _type);

private:

	//敵のパラメータ情報
	std::array<EnemyParameter, static_cast<int>(ENEMY_TYPE::MAX)> parameters_;

	//モデル等のロード
	void LoadModelAndAnimation(EnemyBase& _enemy, const ENEMY_TYPE& _type);
};