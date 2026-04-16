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
		InitPouse();
		InitKeyConfig();
		InitSwitchCntl();
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
}

void ResourceManager::InitClear(void)
{
	Resource res;	
}

void ResourceManager::InitGameOver(void)
{
	Resource res;
}

void ResourceManager::InitPouse(void)
{
	Resource res;
}

void ResourceManager::InitKeyConfig(void)
{
	Resource res;
}

void ResourceManager::InitSwitchCntl(void)
{
	Resource res;
}

void ResourceManager::ResorceDeviceIcon(void)
{
	Resource res;
}

void ResourceManager::ResorceStage(void)
{
	Resource res;
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
