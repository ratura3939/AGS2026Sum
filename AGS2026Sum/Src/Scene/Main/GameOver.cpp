#include"../../pch.h"
#include"../../Application.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include"Title.h"

#include "GameOver.h"

GameOver::GameOver(void)
{
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
	//リソース準備
	ResourceManager& rsM = ResourceManager::GetInstance();
	rsM.GetInstance().Init(SceneManager::SCENE_ID::GAMEOVER);

	//UI初期化
	InitUI();

	//レンダーとマテリアル(背景の引き伸ばし用)
	material_ = std::make_unique<PixelMaterial>(L"NomalTexPS.cso", 0);
	material_->AddTextureBuf(rsM.Load(ResourceManager::SRC::GAME_OVER_BACK_IMG).handleId_);

	render_ = std::make_unique<PixelRenderer>(*material_);

	auto& app = Application::GetInstance();
	render_->MakeSquereVertex({ 0,0 }, { app.GetWindowWidth(), app.GetWindowHeight() });

	//音関係初期化
	InitSound();

	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetPos({ 0.0f,0.0f,0.0f });
}

void GameOver::InitUI(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& rsM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;
}

void GameOver::Update(void)
{
}

void GameOver::Draw(void)
{
	//背景描画
	render_->Draw();
}

void GameOver::Release(void)
{
}

void GameOver::Reset(void)
{
}

void GameOver::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
}

void GameOver::InitEffect(void)
{
}
