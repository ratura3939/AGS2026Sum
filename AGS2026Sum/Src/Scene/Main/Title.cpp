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
#include"Game.h"

#include "Title.h"


//ローカル定数
namespace {
	const float BUTTON_ALPHA_ACC = 8.0f;	//ボタンのα値の加速度
	const float BUTTON_ALPHA_MAX = 255.0f;	//α最大値
	const float BUTTON_ALPHA_MIN = 100.0f;	//α最小値
	const float BUTTON_SCALE = 0.7f;		//大きさ
	const int BGM_VOLUME = 80;		//BGM音量

	const VECTOR BUTTON_START_RELATIVE_CENTER = { 0.0f,100.0f,0.0f };
	const VECTOR BUTTON_END_RELATIVE_CENTER = { 0.0f,200.0f,0.0f };
}

Title::Title(void)
	:isSelectGameEnd_(false)
{
}

Title::~Title(void)
{
}

void Title::Init(void)
{
	//リソース準備
	ResourceManager& rsM = ResourceManager::GetInstance();
	rsM.GetInstance().Init(SceneManager::SCENE_ID::TITLE);

	//コントローラー両対応
	SceneManager::GetInstance().SetController(SceneManager::CNTL::NONE);

	//UI初期化
	InitUI();

	//レンダーとマテリアル(背景の引き伸ばし用)
	material_ = std::make_unique<PixelMaterial>(L"NomalTexPS.cso", 0);
	material_->AddTextureBuf(rsM.Load(ResourceManager::SRC::TITLE_BACK_IMG).handleId_);

	render_ = std::make_unique<PixelRenderer>();
	auto& app = Application::GetInstance();
	render_->MakeSquereVertex({ 0,0 }, { app.GetWindowWidth(), app.GetWindowHeight() });

	//音関係初期化
	InitSound();

	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetPos({ 0.0f,0.0f,0.0f });
}

void Title::InitUI(void)
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

	//スタートボタン設定
	uiM.Add(UI_NAME::START_BUTTON, resM.Load(SOURCE::START_BUTTON_IMG).handleId_, UI_DIREC::FLASHING, UI_DIMENSION::DIMENSION_2);	//登録
	uiM.SetUIInfo(UI_NAME::START_BUTTON, VAdd(screenCenter, BUTTON_START_RELATIVE_CENTER), BUTTON_SCALE);							//基礎設定
	uiM.SetUIDirectionPram(UI_NAME::START_BUTTON, UI_GROUP::GRADUALLY, BUTTON_ALPHA_ACC, BUTTON_ALPHA_MAX, BUTTON_ALPHA_MIN);		//演出設定

	//スタートボタン設定
	uiM.Add(UI_NAME::END_BUTTON, resM.Load(SOURCE::END_BUTTON_IMG).handleId_, UI_DIREC::FLASHING, UI_DIMENSION::DIMENSION_2);	//登録
	uiM.SetUIInfo(UI_NAME::END_BUTTON, VAdd(screenCenter, BUTTON_END_RELATIVE_CENTER), BUTTON_SCALE);							//基礎設定
	uiM.SetUIDirectionPram(UI_NAME::END_BUTTON, UI_GROUP::GRADUALLY, BUTTON_ALPHA_ACC, BUTTON_ALPHA_MAX, BUTTON_ALPHA_MIN);		//演出設定
}

void Title::InitSound(void)
{
	ResourceManager& resM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	using SND_TYPE = SoundManager::TYPE;
	using SND_NAME = SoundManager::SOUND_NAME;

	//決定
	sndM.Add(SND_TYPE::SE, SND_NAME::ENTER_SE,
		resM.Load(ResourceManager::SRC::ENTER_SE).handleId_);

	//カーソル音
	sndM.Add(SND_TYPE::SE, SND_NAME::MOVE_CUSUR_SE,
		resM.Load(ResourceManager::SRC::MOVE_CURSUR_SE).handleId_);

	////BGM
	sndM.Add(SoundManager::TYPE::BGM, SoundManager::SOUND_NAME::TITLE_BGM,
		resM.Load(ResourceManager::SRC::TITLE_BGM).handleId_);

	sndM.AdjustVolume(SND_TYPE::BGM, BGM_VOLUME);	//BGM音量調整

	//BGM再生
	sndM.Play(SoundManager::SOUND_NAME::TITLE_BGM);
}

void Title::InitEffect(void)
{
}

void Title::ResetUIDirectionParam(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_NAME = UIManager2d::UI_NAME;

	uiM.SetAlpha(UI_NAME::START_BUTTON, BUTTON_ALPHA_MAX);
	uiM.SetAlpha(UI_NAME::END_BUTTON, BUTTON_ALPHA_MAX);
}

void Title::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	if (ins.IsTriggerDown(InputManager::INPUT_COMMAND::ENTER))
	{
		sndM.Play(SoundManager::SOUND_NAME::ENTER_SE);
		if (isSelectGameEnd_) {
			Application::GetInstance().EndGame();
			return;	//ゲーム終了
		}

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(std::make_shared<Game>());
	}

	//ボタン選択
	if (ins.IsTriggerDown(InputManager::INPUT_COMMAND::UP) && isSelectGameEnd_) {
		isSelectGameEnd_ = false;
		ResetUIDirectionParam();
		sndM.Play(SoundManager::SOUND_NAME::MOVE_CUSUR_SE);

	}
	else if (ins.IsTriggerDown(InputManager::INPUT_COMMAND::DOWN) && !isSelectGameEnd_) {
		isSelectGameEnd_ = true;
		ResetUIDirectionParam();
		sndM.Play(SoundManager::SOUND_NAME::MOVE_CUSUR_SE);
	}

	//UI更新
	UIManager2d::UI_NAME directionButton = UIManager2d::UI_NAME::START_BUTTON;
	if (isSelectGameEnd_) {
		directionButton = UIManager2d::UI_NAME::END_BUTTON;
	}

	//動きのあるUIの更新
	UIManager2d::GetInstance().Update(directionButton);
	
}

void Title::Draw(void)
{
	auto& uiM = UIManager2d::GetInstance();

	//背景描画
	render_->Draw(*material_);

	uiM.Draw(UIManager2d::UI_NAME::START_BUTTON);
	uiM.Draw(UIManager2d::UI_NAME::END_BUTTON);
}

void Title::Release(void)
{
	SoundManager& sndM = SoundManager::GetInstance();

	//BGMを止める
	sndM.StopAll(SoundManager::SOUND_NAME::TITLE_BGM);	//今まで流していたものを停止
}

void Title::Reset(void)
{
}
