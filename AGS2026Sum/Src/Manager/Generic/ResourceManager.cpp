#include"../../pch.h"
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(SceneManager::SCENE_ID _scene)
{
	//シーンごとのリソースと生成する可能性がある追加シーンのリソース
	switch (_scene)
	{
	case SceneManager::SCENE_ID::TITLE:
		InitTitle();
		break;
	case SceneManager::SCENE_ID::GAME:
		InitGame();
		InitPause();
		break;
	case SceneManager::SCENE_ID::GAMEOVER:
		InitGameOver();
		break;
	case SceneManager::SCENE_ID::CLEAR:
		InitClear();
		break;
	default:
		break;
	}
}


void ResourceManager::InitTitle(void)
{
	Resource res;

	// タイトル
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + L"Title.png");
	resourcesMap_.emplace(SRC::TITLE_BACK_IMG, res);

	// スタートボタン
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"StartButton.png");
	resourcesMap_.emplace(SRC::START_BUTTON_IMG, res);

	// 終了
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"EndButton.png");
	resourcesMap_.emplace(SRC::END_BUTTON_IMG, res);

	//UIインフォ(α②終了後、UIの情報管理について考える)
	//res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"SceneParam/TitleUIDirectionInfo.json");
	//resourcesMap_.emplace(SRC::TITLE_UI_INFO_JSON, res);
}

void ResourceManager::InitGame(void)
{
	Resource res;

	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + L"Tutorial.mv1");
	resourcesMap_.emplace(SRC::STAGE_MDL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + L"BossStage.mv1");
	resourcesMap_.emplace(SRC::BOSS_STAGE_MDL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + L"DoorRight.mv1");
	resourcesMap_.emplace(SRC::DOOR_RIGHT_MDL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + L"DoorLeft.mv1");
	resourcesMap_.emplace(SRC::DOOR_LEFT_MDL, res);

	res = Resource(Resource::TYPE::IMGS, Application::PATH_UI + L"comboNumber.png", 5, 2, 52, 64);
	resourcesMap_.emplace(SRC::COMBO_NUMBER_IMG, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"ComboUI.png");
	resourcesMap_.emplace(SRC::COMBO_STRING_IMG, res);

	ResourcePlayer();	//プレイヤー関連

	ResourceEnemy();	//敵関連

	ResourceBoss();		//ボス関連
}

void ResourceManager::InitClear(void)
{
	Resource res;	

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + L"GameClear.png");
	resourcesMap_.emplace(SRC::GAME_CLEAR_BACK_IMG, res);

	// 終了
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"EndButton.png");
	resourcesMap_.emplace(SRC::END_BUTTON_IMG, res);

	// タイトルに戻る
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"BackTitleButton.png");
	resourcesMap_.emplace(SRC::BACK_TITLE_IMG, res);
}

void ResourceManager::InitGameOver(void)
{
	Resource res;

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + L"GameOver.png");
	resourcesMap_.emplace(SRC::GAME_OVER_BACK_IMG, res);

	// 終了
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"EndButton.png");
	resourcesMap_.emplace(SRC::END_BUTTON_IMG, res);

	// タイトルに戻る
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"BackTitleButton.png");
	resourcesMap_.emplace(SRC::BACK_TITLE_IMG, res);
}

void ResourceManager::InitPause(void)
{
	Resource res;

	// タイトルに戻る
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"BackTitleButton.png");
	resourcesMap_.emplace(SRC::BACK_TITLE_IMG, res);

	// ゲームに戻る
	res = Resource(Resource::TYPE::IMG, Application::PATH_UI + L"ReturnGameButton.png");
	resourcesMap_.emplace(SRC::BACK_GAME_IMG, res);
}

void ResourceManager::ResourcePlayer(void)
{
	Resource res;
	//モデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + L"Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MDL, res);

	//固有アニメーション
	//***************************************************************************************
	//待機
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/Idle.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE_ANIM, res);
	//ダッシュ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN_ANIM, res);
	//ダメージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/HitDamage.mv1");
	resourcesMap_.emplace(SRC::PLAYER_DAMAGE_ANIM, res);
	//パンチ１
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/FirstPunch.mv1");
	resourcesMap_.emplace(SRC::PLAYER_FIRST_PUNCH_ANIM, res);
	//パンチ２
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/SecondPunch.mv1");
	resourcesMap_.emplace(SRC::PLAYER_SECOND_PUNCH_ANIM, res);
	//パンチ３
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/FinishPunch.mv1");
	resourcesMap_.emplace(SRC::PLAYER_THIRD_PUNCH_ANIM, res);
	//キック１
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/MiddleKick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MIDDLE_KICK_ANIM, res);
	//キック２
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/HighKick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_HIGH_KICK_ANIM, res);
	//キック３
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/HurricanKick.mv1");
	resourcesMap_.emplace(SRC::PLAYER_FINISH_KICK_ANIM, res);
	//特殊攻撃(パンチ派生)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/SpecialAttack(Jump).mv1");
	resourcesMap_.emplace(SRC::PLAYER_SPECIAL_PUNCH_ANIM, res);
	//特殊攻撃(キック派生)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/SpecialAttack(nagiharai).mv1");
	resourcesMap_.emplace(SRC::PLAYER_SPECIAL_KICK_ANIM, res);
	//必殺技
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/UltimetAttack.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ULTIMET_ANIM, res);
	//必殺技(テスト用)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/UltimetAttack(tukami).mv1");
	resourcesMap_.emplace(SRC::PLAYER_ULTIMET_TEST_ANIM, res);

	//効果音
	//攻撃
	//パンチ初段
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + L"Punch_First_Test.mp3");
	resourcesMap_.emplace(SRC::PUNCH_FIRST_PLAYER_SE, res);

	//パンチ二段目
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + L"Punch_Second_Test.mp3");
	resourcesMap_.emplace(SRC::PUNCH_SECOND_PLAYER_SE, res);

	//パンチ最終段
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + L"Punch_Third_Test.mp3");
	resourcesMap_.emplace(SRC::PUNCH_THIRD_PLAYER_SE, res);

	//キック初段
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + L"Kick_First_Test.mp3");
	resourcesMap_.emplace(SRC::KICK_FIRST_PLAYER_SE, res);

	//キック二段目
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + L"Kick_Second_Test.mp3");
	resourcesMap_.emplace(SRC::KICK_SECOND_PLAYER_SE, res);

	//キック最終段
	res = Resource(Resource::TYPE::SOUND, Application::PATH_SE + L"Kick_Third_Test.mp3");
	resourcesMap_.emplace(SRC::KICK_THIRD_PLAYER_SE, res);

	//エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + L"PunchThird.efk");
	resourcesMap_.emplace(SRC::PLAYER_PUNCH_THIRD_EFC, res);

	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + L"KickThird.efk");
	resourcesMap_.emplace(SRC::PLAYER_KICK_THIRD_EFC, res);

	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + L"PunchSpecial.efk");
	resourcesMap_.emplace(SRC::PLAYER_PUNCH_SPECIAL_EFC, res);

	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + L"Ultimate.efk");
	resourcesMap_.emplace(SRC::PLAYER_ULTIMATE_EFC, res);

	//JSON
	res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"PlayerAttackData.json");
	resourcesMap_.emplace(SRC::PLAYER_ATTACK_DATA_JSON, res);

	res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"PlayerAttackDirectionInfo.json");
	resourcesMap_.emplace(SRC::PLAYER_ATTACK_DIRECTION_DATA_JSON, res);
}

void ResourceManager::ResourceEnemy(void)
{
	Resource res;

	//敵モデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + L"Enemy.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MDL, res);

	//敵アニメーション
	std::wstring animPath = Application::PATH_ANIMATION + L"Enemy/";

	//待機
	res = Resource(Resource::TYPE::MODEL, animPath + L"Idle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_IDLE_ANIM, res);

	//歩く
	res = Resource(Resource::TYPE::MODEL, animPath + L"Walk.mv1");
	resourcesMap_.emplace(SRC::ENEMY_WALK_ANIM, res);

	//走る
	res = Resource(Resource::TYPE::MODEL, animPath + L"Run.mv1");
	resourcesMap_.emplace(SRC::ENEMY_RUN_ANIM, res);

	//攻撃
	res = Resource(Resource::TYPE::MODEL, animPath + L"Attack.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ATTACK_ANIM, res);

	//突進
	res = Resource(Resource::TYPE::MODEL, animPath + L"Tackle.mv1");
	resourcesMap_.emplace(SRC::ENEMY_TACKLE_ANIM, res);

	//ジャンプ
	res = Resource(Resource::TYPE::MODEL, animPath + L"Jumping.mv1");
	resourcesMap_.emplace(SRC::ENEMY_JUMP_ANIM, res);

	//よろけ
	res = Resource(Resource::TYPE::MODEL, animPath + L"Stagger.mv1");
	resourcesMap_.emplace(SRC::ENEMY_STAGGER_ANIM, res);

	//打ち上げ
	res = Resource(Resource::TYPE::MODEL, animPath + L"Launch.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LAUNCH_ANIM, res);

	//打ち上げ倒れ
	res = Resource(Resource::TYPE::MODEL, animPath + L"LaunchDown.mv1");
	resourcesMap_.emplace(SRC::ENEMY_LAUNCH_DOWN_ANIM, res);

	//吹っ飛び前半
	res = Resource(Resource::TYPE::MODEL, animPath + L"BlowFirstHalf.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BLOW_FIRST_HALF_ANIM, res);

	//吹っ飛び後半
	res = Resource(Resource::TYPE::MODEL, animPath + L"BlowSecondHalf.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BLOW_SECOND_HALF_ANIM, res);

	//吹っ飛び終了
	res = Resource(Resource::TYPE::MODEL, animPath + L"BlowEnd.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BLOW_END_ANIM, res);

	//死亡
	res = Resource(Resource::TYPE::MODEL, animPath + L"Death.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DEATH_ANIM, res);

	//パラメーター
	res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"NormalEnemyParam.json");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_PARAMETER, res);

	//スキル情報
	res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"EnemySkillsData.json");
	resourcesMap_.emplace(SRC::ENEMY_SKILLS_DATA, res);
}

void ResourceManager::ResourceBoss(void)
{
	Resource res;

	//ボスモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + L"MiddleBoss.mv1");
	resourcesMap_.emplace(SRC::MIDDLE_BOSS_MDL, res);

	//パラメーター
	res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"MiddleBossParam.json");
	resourcesMap_.emplace(SRC::MIDDLE_BOSS_PARAMETER, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
