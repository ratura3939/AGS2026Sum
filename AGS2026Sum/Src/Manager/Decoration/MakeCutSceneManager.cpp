#include "../../pch.h"
#include"../../Application.h"
#include"MakeCutSceneElement/MakeCutSceneElementBase.h"
#include"MakeCutSceneElement/StartUp.h"
#include"MakeCutSceneElement/EditDataScene.h"
#include "MakeCutSceneManager.h"

namespace{
	const int WINDOW_SHOW_IDX = 1;
	const int WINDOW_HIDE_IDX = 0;
}

void MakeCutSceneManager::Init(void)
{
	startUp_->Init();
	editData_->Init();
}

void MakeCutSceneManager::Update(void)
{
	//空の時
	if (useFunction_.expired()) {
		//処理しない
		return;
	}

	//更新処理
	useFunction_.lock()->Update();
}

void MakeCutSceneManager::DrawAtMainWindow(void)
{
	//空の時
	if (useFunction_.expired()) {
		//処理しない
		return;
	}

	if (!useMakeCutSceneWindow_) {
		//描画処理
		useFunction_.lock()->Draw();
	}
}

void MakeCutSceneManager::Draw(void)
{
	//空の時
	if (useFunction_.expired()) {
		//処理しない
		return;
	}

	SetScreenFlipTargetWindow(editWindow_); // 専用ウィンドウをターゲットに設定
	SetDrawScreen(editScreen_);

	// 画面を初期化
	ClearDrawScreen();

	//描画処理
	useFunction_.lock()->Draw();

	// 背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, editScreen_, true);

	ScreenFlip();

	SetScreenFlipTargetWindow(NULL);;	//戻す
}

void MakeCutSceneManager::ChangeFunction(const FUNCTION& _command)
{
	switch (_command) {
	case FUNCTION::START_UP:
		useFunction_ = startUp_;
		break;

	case FUNCTION::EDIT:
		useFunction_ = editData_;
		break;
	}

	//ウィンドウが描画されていない時
	if (!useMakeCutSceneWindow_) {
		//必要とするシーンへと遷移するとき
		if (_command != FUNCTION::START_UP) {
			//ウィンドウを表示する
			RedyWindow(true);
		}
	}
	//すでに描画されている時
	else {
		//必要としないシーンへと遷移するとき
		if ( _command == FUNCTION::START_UP) {
			//ウィンドウを非表示する
			RedyWindow(false);
		}
	}

	useFunction_.lock()->Reset();	//必要パラメータ初期化
}

//ウィンドウ関係
//********************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void MakeCutSceneManager::RedyWindow(const bool _showFlag)
{
	//ウィンドウの状態設定
	int windowModeIdx = WINDOW_HIDE_IDX;

	if (_showFlag) {
		windowModeIdx = WINDOW_SHOW_IDX;
	}

	ShowWindow(editWindow_, windowModeIdx);
	//使用フラグオン
	useMakeCutSceneWindow_ = _showFlag;
	//マウス表示設定
	SetMouseDispFlag(_showFlag);
	//両方のウィンドウをアクティブ設定
	SetAlwaysRunFlag(_showFlag);
}

void MakeCutSceneManager::MakeWindow(void)
{
	LPCWSTR windowName = L"MakeCutScene";
	LPCWSTR className = L"SubWindow";

	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS myWindow;
	myWindow.style = CS_HREDRAW | CS_VREDRAW;
	myWindow.lpfnWndProc = WndProc;
	myWindow.cbClsExtra = 0;
	myWindow.cbWndExtra = 0;
	myWindow.hInstance = hInstance;
	myWindow.hIcon = NULL;
	myWindow.hCursor = LoadCursor(NULL, IDC_ARROW);
	myWindow.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	myWindow.lpszMenuName = NULL;
	myWindow.lpszClassName = className;
	if (!RegisterClass(&myWindow))
	{
		return;
	}

	Application& app = Application::GetInstance();

	int windowWidth = app.GetWindowWidth();
	int windowHeight = app.GetWindowHeight();

	RECT windowRect = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	editWindow_ = CreateWindow(
		className,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, NULL, hInstance, NULL
	);
	const int posX = windowWidth;
	const int posY = 0;
	SetWindowPos(editWindow_, NULL, posX, posY, posX + windowWidth, posY + windowHeight, NULL);

	//スクリーンも生成
	editScreen_ = MakeScreen(
		windowWidth, windowHeight, true);
}

MakeCutSceneManager::MakeCutSceneManager(void)
	:useMakeCutSceneWindow_(false)
	,editScreen_(-1)
	,startUp_(std::make_shared<StartUp>())
	,editData_(std::make_shared<EditDataScene>())
	,useFunction_()
{
	MakeWindow();
	ChangeFunction(FUNCTION::START_UP);
}


MakeCutSceneManager::~MakeCutSceneManager(void)
{
	DestroyWindow(editWindow_);
}
