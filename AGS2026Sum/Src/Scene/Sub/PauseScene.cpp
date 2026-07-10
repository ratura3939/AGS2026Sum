#include"../pch.h"
#include"../../Application.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Scene/Main/Title.h"
#include"../../Utility/Utility.h"

#include "PauseScene.h"

//ローカル定数
namespace {
	const float BUTTON_ALPHA_ACC = 8.0f;	//ボタンのα値の加速度
	const float BUTTON_ALPHA_MAX = 255.0f;	//α最大値
	const float BUTTON_ALPHA_MIN = 100.0f;	//α最小値
	const float BUTTON_SCALE = 0.7f;		//大きさ

	const VECTOR BUTTON_TOGAME_RELATIVE_CENTER = { 0.0f,-100.0f,0.0f };
	const VECTOR BUTTON_TOTITLE_RELATIVE_CENTER = { 0.0f,100.0f,0.0f };
}

PauseScene::PauseScene(void)
	:isSelectBackTitle_(false)
{
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::Init(void)
{
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);

	UIManager2d& uiM = UIManager2d::GetInstance();
	ResourceManager& resM = ResourceManager::GetInstance();
	using UI_DIREC = UIManager2d::UI_DIRECTION_2D;
	using UI_GROUP = UIManager2d::UI_DIRECTION_GROUP;
	using UI_DIMENSION = UIManager2d::UI_DRAW_DIMENSION;

	auto& app = Application::GetInstance();
	VECTOR screenSize = { static_cast<float>(app.GetWindowWidth()),static_cast<float>(app.GetWindowHeight()),0.0f };
	
	//演出初期化
	InitSound();
	InitEffect();

	InitUI();
}

void PauseScene::InitUI(void)
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

	//ゲームに戻るボタン設定
	uiM.Add(UI_NAME::BACK_GAME_BUTTON, resM.Load(SOURCE::BACK_GAME_IMG).handleId_, UI_DIREC::FLASHING, UI_DIMENSION::DIMENSION_2);	//登録
	uiM.SetUIInfo(UI_NAME::BACK_GAME_BUTTON, VAdd(screenCenter, BUTTON_TOGAME_RELATIVE_CENTER), BUTTON_SCALE);							//基礎設定
	uiM.SetUIDirectionPram(UI_NAME::BACK_GAME_BUTTON, UI_GROUP::GRADUALLY, BUTTON_ALPHA_ACC, BUTTON_ALPHA_MAX, BUTTON_ALPHA_MIN);		//演出設定

	//対等に戻るボタン設定
	uiM.Add(UI_NAME::BACK_TITLE_BUTTON, resM.Load(SOURCE::BACK_TITLE_IMG).handleId_, UI_DIREC::FLASHING, UI_DIMENSION::DIMENSION_2);	//登録
	uiM.SetUIInfo(UI_NAME::BACK_TITLE_BUTTON, VAdd(screenCenter, BUTTON_TOTITLE_RELATIVE_CENTER), BUTTON_SCALE);							//基礎設定
	uiM.SetUIDirectionPram(UI_NAME::BACK_TITLE_BUTTON, UI_GROUP::GRADUALLY, BUTTON_ALPHA_ACC, BUTTON_ALPHA_MAX, BUTTON_ALPHA_MIN);		//演出設定
}

void PauseScene::InitSound(void)
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
}

void PauseScene::InitEffect(void)
{
	//随時追加
}

void PauseScene::Update(void)
{
	//入力受付
	InputUser();
}

void PauseScene::InputUser(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	InputManager& ins = InputManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	using COMMAND = InputManager::INPUT_COMMAND;

	if (ins.IsTriggerDown(COMMAND::ENTER))
	{
		sndM.Play(SoundManager::SOUND_NAME::ENTER_SE);
		if (isSelectBackTitle_) {
			//シーン遷移
			SceneManager::GetInstance().ChangeScene(std::make_shared<Title>());
			return;
		}

		//ポップして前のシーンに戻る
		SceneManager::GetInstance().PopScene();
	}

	//ボタン選択
	if (ins.IsTriggerDown(COMMAND::UP) && isSelectBackTitle_) {
		isSelectBackTitle_ = false;
		ResetUIDirectionParam();
		sndM.Play(SoundManager::SOUND_NAME::MOVE_CUSUR_SE);
	}
	else if (ins.IsTriggerDown(COMMAND::DOWN) && !isSelectBackTitle_) {
		isSelectBackTitle_ = true;
		ResetUIDirectionParam();
		sndM.Play(SoundManager::SOUND_NAME::MOVE_CUSUR_SE);
	}

	//UI更新
	UIManager2d::UI_NAME directionButton = UIManager2d::UI_NAME::BACK_GAME_BUTTON;
	if (isSelectBackTitle_) {
		directionButton = UIManager2d::UI_NAME::BACK_TITLE_BUTTON;
	}

	//動きのあるUIの更新
	UIManager2d::GetInstance().Update(directionButton);
}

void PauseScene::ResetUIDirectionParam(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();
	using UI_NAME = UIManager2d::UI_NAME;

	uiM.SetAlpha(UI_NAME::BACK_GAME_BUTTON, BUTTON_ALPHA_MAX);
	uiM.SetAlpha(UI_NAME::BACK_TITLE_BUTTON, BUTTON_ALPHA_MAX);
}

void PauseScene::Draw(void)
{
	UIManager2d& uiM = UIManager2d::GetInstance();

	//追加シーンなのでうっすらと背景であるゲームシーンを映るようにする。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 2);
	auto& app = Application::GetInstance();
	DrawBox(0, 0, app.GetWindowWidth(), app.GetWindowHeight(), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	uiM.Draw(UIManager2d::UI_NAME::BACK_GAME_BUTTON);
	uiM.Draw(UIManager2d::UI_NAME::BACK_TITLE_BUTTON);
}

void PauseScene::Release(void)
{
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void PauseScene::Reset(void)
{
	//ポーズシーンではBGMを流さない
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, 0);	//前シーンで流れているBGMの音量を０に
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);
}