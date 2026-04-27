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

	//敵モデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + L"Enemy.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MDL, res);

	//ボスモデル
	res = Resource(Resource::TYPE::MODEL, Application::PATH_CHARACTER + L"Boss.mv1");
	resourcesMap_.emplace(SRC::BOSS_MDL, res);
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
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/PlayerIdleAnimation.mv1");
	resourcesMap_.emplace(SRC::PLAYER_IDLE_ANIM, res);
	//ダッシュ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ANIMATION + L"Player/Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN_ANIM, res);
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
