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

namespace {
	const float BUTTON_ALPHA_ACC = 8.0f;	//ボタンのα値の加速度
	const float BUTTON_ALPHA_MAX = 255.0f;	//α最大値
	const float BUTTON_ALPHA_MIN = 100.0f;	//α最小値
	const float BUTTON_SCALE = 0.7f;		//大きさ

	const VECTOR BUTTON_TOTITLE_RELATIVE_CENTER = { -200.0f,200.0f,0.0f };
	const VECTOR BUTTON_END_RELATIVE_CENTER = { 200.0f,200.0f,0.0f };
}

GameOver::GameOver(void)
	:isSelectEndGame_(false)
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

	render_ = std::make_unique<PixelRenderer>();

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
	ResourceManager& resM = ResourceManager::GetInstance();

	using SOURCE = ResourceManager::SRC;

	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;
	using UI_NAME = UIManager2d::UI_NAME;

	//画面中央取得
	auto& app = Application::GetInstance();
	const VECTOR screenCenter = { static_cast<float>(app.GetWindowWidth() / 2),static_cast<float>(app.GetWindowHeight() / 2),0.0f };

	//タイトルに戻るボタン設定
	uiM.Add(UI_NAME::BACK_TITLE_BUTTON, resM.Load(SOURCE::BACK_TITLE_IMG).handleId_, UI_DIREC::FLASHING, UI_DIMENSION::DIMENSION_2);	//登録
	uiM.SetUIInfo(UI_NAME::BACK_TITLE_BUTTON, VAdd(screenCenter, BUTTON_TOTITLE_RELATIVE_CENTER), BUTTON_SCALE);						//基礎設定
	uiM.SetUIDirectionPram(UI_NAME::BACK_TITLE_BUTTON, UI_GROUP::GRADUALLY, BUTTON_ALPHA_ACC, BUTTON_ALPHA_MAX, BUTTON_ALPHA_MIN);		//演出設定

	//スタートボタン設定
	uiM.Add(UI_NAME::END_BUTTON, resM.Load(SOURCE::END_BUTTON_IMG).handleId_, UI_DIREC::FLASHING, UI_DIMENSION::DIMENSION_2);	//登録
	uiM.SetUIInfo(UI_NAME::END_BUTTON, VAdd(screenCenter, BUTTON_END_RELATIVE_CENTER), BUTTON_SCALE);							//基礎設定
	uiM.SetUIDirectionPram(UI_NAME::END_BUTTON, UI_GROUP::GRADUALLY, BUTTON_ALPHA_ACC, BUTTON_ALPHA_MAX, BUTTON_ALPHA_MIN);		//演出設定
}

void GameOver::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTriggerDown(InputManager::INPUT_COMMAND::ENTER))
	{
		if (isSelectEndGame_) {
			Application::GetInstance().EndGame();
			return;	//ゲーム終了
		}

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(std::make_shared<Title>());
	}

	//ボタン選択
	if (ins.IsTriggerDown(InputManager::INPUT_COMMAND::LEFT) && isSelectEndGame_) {
		isSelectEndGame_ = false;
		ResetUIDirectionParam();
	}
	else if (ins.IsTriggerDown(InputManager::INPUT_COMMAND::RIGHT) && !isSelectEndGame_) {
		isSelectEndGame_ = true;
		ResetUIDirectionParam();
	}

	//UI更新
	UIManager2d::UI_NAME directionButton = UIManager2d::UI_NAME::BACK_TITLE_BUTTON;
	if (isSelectEndGame_) {
		directionButton = UIManager2d::UI_NAME::END_BUTTON;
	}

	//動きのあるUIの更新
	UIManager2d::GetInstance().Update(directionButton);
}

void GameOver::Draw(void)
{
	//背景描画
	render_->Draw(*material_);

	auto& uiM = UIManager2d::GetInstance();
	uiM.Draw(UIManager2d::UI_NAME::BACK_TITLE_BUTTON);
	uiM.Draw(UIManager2d::UI_NAME::END_BUTTON);
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

void GameOver::ResetUIDirectionParam(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_NAME = UIManager2d::UI_NAME;

	uiM.SetAlpha(UI_NAME::BACK_TITLE_BUTTON, BUTTON_ALPHA_MAX);
	uiM.SetAlpha(UI_NAME::END_BUTTON, BUTTON_ALPHA_MAX);
}
