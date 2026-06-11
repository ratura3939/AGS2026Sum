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
	//res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	//resourcesMap_.emplace(SRC::TITLE_LOGO, res);
}

void ResourceManager::InitGame(void)
{
	Resource res;

	ResourcePlayer();	//プレイヤー関連

	ResourceEnemy();	//敵関連

	ResourceBoss();		//ボス関連
}

void ResourceManager::InitClear(void)
{
	Resource res;	
}

void ResourceManager::InitGameOver(void)
{
	Resource res;
}

void ResourceManager::InitPause(void)
{
	Resource res;
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

	//JSON
	res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"PlayerAttackData.json");
	resourcesMap_.emplace(SRC::PLAYER_ATTACK_DATA_JSON, res);
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

	//吹っ飛び
	res = Resource(Resource::TYPE::MODEL, animPath + L"Blow.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BLOW_ANIM, res);

	//死亡
	res = Resource(Resource::TYPE::MODEL, animPath + L"Death.mv1");
	resourcesMap_.emplace(SRC::ENEMY_DEATH_ANIM, res);

	//パラメーター
	res = Resource(Resource::TYPE::JSON, Application::PATH_JSON + L"NormalEnemyParam.json");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_PARAMETER, res);
}

void ResourceManager::ResourceBoss(void)
{
	Resource res;

	//ボスモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + L"MiddleBoss.mv1");
	resourcesMap_.emplace(SRC::MIDDLE_BOSS_MDL, res);
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
