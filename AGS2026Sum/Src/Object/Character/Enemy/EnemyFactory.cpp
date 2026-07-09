#include "../../../pch.h"
#include "../../../Utility/Utility.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/GameSystem/AnimationController.h"
#include "Skill/EnemySkillBase.h"
#include "Types/NormalEnemy.h"
#include "Types/Boss/MiddleBoss.h"
#include "EnemyManager.h"
#include "EnemyBase.h"
#include "EnemyFactory.h"

//ヘッダー内に余計なマネージャーインクルードをしないために、namespaceにて記載
namespace
{
	//リソース名のテーブル
	const std::array<
		std::unordered_map<std::wstring, ResourceManager::SRC>,
		static_cast<int>(ENEMY_TYPE::MAX)
	> SRC_TABLE =
	{
		//雑魚
		std::unordered_map<std::wstring, ResourceManager::SRC>
		{
			{L"ModelName",   ResourceManager::SRC::ENEMY_MDL},
			{L"Idle",   ResourceManager::SRC::ENEMY_IDLE_ANIM},
			{L"Walk",   ResourceManager::SRC::ENEMY_WALK_ANIM},
			{L"Run",    ResourceManager::SRC::ENEMY_RUN_ANIM},
			{L"Attack", ResourceManager::SRC::ENEMY_ATTACK_ANIM},
			{L"Stagger", ResourceManager::SRC::ENEMY_STAGGER_ANIM},
			{L"Launch", ResourceManager::SRC::ENEMY_LAUNCH_ANIM},
			{L"LaunchDown", ResourceManager::SRC::ENEMY_LAUNCH_DOWN_ANIM},
			{L"BlowFirstHalf", ResourceManager::SRC::ENEMY_BLOW_FIRST_HALF_ANIM},
			{L"BlowSecondHalf", ResourceManager::SRC::ENEMY_BLOW_SECOND_HALF_ANIM},
			{L"BlowEnd", ResourceManager::SRC::ENEMY_BLOW_END_ANIM}
		},

		//中ボス
		std::unordered_map<std::wstring, ResourceManager::SRC>
		{
			{L"ModelName", ResourceManager::SRC::MIDDLE_BOSS_MDL},
			{ L"Idle",   ResourceManager::SRC::ENEMY_IDLE_ANIM },
			{L"Walk",   ResourceManager::SRC::ENEMY_WALK_ANIM},
			{L"Run",    ResourceManager::SRC::ENEMY_RUN_ANIM},
			{L"Attack", ResourceManager::SRC::ENEMY_ATTACK_ANIM},
			{L"Stagger", ResourceManager::SRC::ENEMY_STAGGER_ANIM},
			{L"Launch", ResourceManager::SRC::ENEMY_LAUNCH_ANIM},
			{L"LaunchDown", ResourceManager::SRC::ENEMY_LAUNCH_DOWN_ANIM},
			{L"BlowFirstHalf", ResourceManager::SRC::ENEMY_BLOW_FIRST_HALF_ANIM},
			{L"BlowSecondHalf", ResourceManager::SRC::ENEMY_BLOW_SECOND_HALF_ANIM},
			{L"BlowEnd", ResourceManager::SRC::ENEMY_BLOW_END_ANIM},
			{L"Tackle", ResourceManager::SRC::ENEMY_TACKLE_ANIM},
			{L"Jump", ResourceManager::SRC::ENEMY_JUMP_ANIM}
		}
	};
}

EnemyFactory::EnemyFactory(void)
{
	create_[static_cast<int>(ENEMY_TYPE::NORMAL)] = []()->std::unique_ptr<EnemyBase> {return std::make_unique<NormalEnemy>();};
	create_[static_cast<int>(ENEMY_TYPE::MIDDLE_BOSS)] = []()->std::unique_ptr<EnemyBase> {return std::make_unique<MiddleBoss>();};
}

EnemyFactory::~EnemyFactory(void)
{
}

void EnemyFactory::Load(void)
{
	//リソース
	auto& res = ResourceManager::GetInstance();

	//外部ファイル取得
	parameters_[static_cast<int>(ENEMY_TYPE::NORMAL)] = res.Load(ResourceManager::SRC::NORMAL_ENEMY_PARAMETER).GetData<EnemyParameter>();
	parameters_[static_cast<int>(ENEMY_TYPE::MIDDLE_BOSS)] = res.Load(ResourceManager::SRC::MIDDLE_BOSS_PARAMETER).GetData<EnemyParameter>();
	animDatas_ = res.Load(ResourceManager::SRC::ENEMY_ANIMATION_DATA).GetData<EnemyAnimationData>();
}

std::unique_ptr<EnemyBase> EnemyFactory::CreateNewEnemy(const ENEMY_TYPE& _type)
{
	//敵の生成
	std::unique_ptr<EnemyBase> enemy = create_[static_cast<int>(_type)]();

	//モデルとアニメーションのロード
	LoadModelAndAnimation(*enemy, _type);

	//ロード
	enemy->Load();

	//初期化
	enemy->Init();

	//動的パラメーターの初期化
	enemy->InitRunTimeParameter(parameters_[static_cast<int>(_type)]);

	//スキル生成
	CreateSkill(*enemy, _type);

	//完成品を返す
	return enemy;
}

const EnemyParameter& EnemyFactory::GetParam(const ENEMY_TYPE& _type) const
{
	return parameters_[static_cast<int>(_type)];
}

void EnemyFactory::LoadModelAndAnimation(EnemyBase& _enemy, const ENEMY_TYPE& _type)
{
	//リソース
	auto& res = ResourceManager::GetInstance();
	int animModel = -1;
	int type = static_cast<int>(_type);

	//参照パラメータ
	const auto& param = parameters_[static_cast<int>(_type)];
	const auto& animData = animDatas_.info[param.modelName];

	//モデルID
	int model = res.LoadModelDuplicate(SRC_TABLE[type].at(L"ModelName"));

	//アニメーション
	std::unique_ptr<AnimationController> anim = std::make_unique<AnimationController>(model);

	//モデル
	_enemy.SetModel(model);

	//メインボーン
	anim->SetRootFrameIndex(Utility::StringToWstring(animData.mainFrameName));

	for (auto& [animName, animParam] : animData.animParam)
	{
		//アニメーション名
		std::wstring animNameWstr = Utility::StringToWstring(animName);

		//アニメーションのモデルID
		animModel = res.LoadModelDuplicate(SRC_TABLE[type].at(animNameWstr));

		//ループ
		AnimationController::PLAY_TYPE playType = AnimationController::PLAY_TYPE::NORMAL;
		if (animParam.isLoop)playType = AnimationController::PLAY_TYPE::LOOP;

		//追加
		anim->Add(animNameWstr, animModel, playType, AnimationController::ANIM_SOURCE::EXTERNAL, animParam.isLock, animParam.isFixPos);

		//座標固定
		if (animParam.isFixPos)
		{
			anim->SetFixAnimationAxisInfo(animNameWstr, animParam.fixPos.x, animParam.fixPos.y, animParam.fixPos.z);
		}
	}

	//デフォルトアニメーション設定
	anim->SetDefaultAnim(L"Idle");

	//アニメーション設定
	_enemy.SetAnim(std::move(anim));
}

void EnemyFactory::CreateSkill(EnemyBase& _enemy, const ENEMY_TYPE& _type)
{
	//パラメーター
	const auto& param = parameters_[static_cast<int>(_type)];

	//スキル
	std::vector<std::unique_ptr<EnemySkillBase>> skills;

	//文字列
	for (const std::string& skillName : param.skillName)
	{
		//文字変換
		std::wstring skillNameW = Utility::StringToWstring(skillName);

		//名前に沿ったスキルの取得
		auto skill = skillFactory.CreateSkill(skillNameW);

		//存在したなら格納
		if (skill)skills.push_back(std::move(skill));
	}

	//敵にスキルを知らせる
	_enemy.SetSkills(std::move(skills));
}