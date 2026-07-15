#include "../../pch.h"
#include"../../Application.h"
#include "MakeCutSceneManager.h"

void MakeCutSceneManager::Update(void)
{
	//更新処理
	(this->*updateFunc_)();
}

void MakeCutSceneManager::DrawAtMainWindow(void)
{
	if (!useMakeCutSceneWindow_) {
		//描画処理
		(this->*drawFunc_)();
	}
}

void MakeCutSceneManager::Draw(void)
{
	SetScreenFlipTargetWindow(editWindow_); // 専用ウィンドウをターゲットに設定
	//描画処理
	(this->*drawFunc_)();

	SetScreenFlipTargetWindow(NULL);;	//戻す
}

void MakeCutSceneManager::ChangeFunction(const USER_INPUT_ACTION& _command)
{
	switch (_command) {
	case USER_INPUT_ACTION::NONE:
		updateFunc_ = &MakeCutSceneManager::UpdateStart;
		drawFunc_ = &MakeCutSceneManager::DrawStart;
		break;

	case USER_INPUT_ACTION::SELECT:
		updateFunc_ = &MakeCutSceneManager::UpdateSelectFunction;
		drawFunc_ = &MakeCutSceneManager::DrawSelectFunction;
		break;

	case USER_INPUT_ACTION::MAKE_FILE:
		updateFunc_ = &MakeCutSceneManager::UpdateMakeFile;
		drawFunc_ = &MakeCutSceneManager::DrawMakeFile;
		break;

	case USER_INPUT_ACTION::EDIT:
		updateFunc_ = &MakeCutSceneManager::UpdateEdit;
		drawFunc_ = &MakeCutSceneManager::DrawEdit;
		break;

	case USER_INPUT_ACTION::ADD_DATA:
		updateFunc_ = &MakeCutSceneManager::UpdateAddData;
		drawFunc_ = &MakeCutSceneManager::DrawAddData;
		break;
	}

	//ウィンドウの生成・削除
	if (_command == USER_INPUT_ACTION::NONE) {
		DestroyWindow(editWindow_);
		useMakeCutSceneWindow_ = false;
	}
	else {
		MakeWindow();
		useMakeCutSceneWindow_ = true;
	}
}

void MakeCutSceneManager::UpdateStart(void)
{
	if (CheckHitKey(KEY_INPUT_M)) {
		ChangeFunction(USER_INPUT_ACTION::SELECT);
	}
}

void MakeCutSceneManager::UpdateSelectFunction(void)
{
	if (CheckHitKey(KEY_INPUT_N)) {
		ChangeFunction(USER_INPUT_ACTION::NONE);
	}
}

void MakeCutSceneManager::UpdateMakeFile(void)
{
}

void MakeCutSceneManager::UpdateEdit(void)
{
}

void MakeCutSceneManager::UpdateAddData(void)
{
}

void MakeCutSceneManager::DrawStart(void)
{
	DrawString(0, 0, L"OK!CreateManager", 0xff0000);
}

void MakeCutSceneManager::DrawSelectFunction(void)
{
	DrawString(0, 0, L"OK!!NewWindow", 0xff0000);
}

void MakeCutSceneManager::DrawMakeFile(void)
{
}

void MakeCutSceneManager::DrawEdit(void)
{
}

void MakeCutSceneManager::DrawAddData(void)
{
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

	const int windowShowNomal = 1;
	ShowWindow(editWindow_, windowShowNomal);

	const int posX = windowWidth;
	const int posY = 0;
	SetWindowPos(editWindow_, NULL, posX, posY, posX + windowWidth, posY + windowHeight, NULL);
}

MakeCutSceneManager::MakeCutSceneManager(void)
	:useMakeCutSceneWindow_(false)
	,editScreen_(-1)
	,updateFunc_(&MakeCutSceneManager::UpdateStart)
	,drawFunc_(&MakeCutSceneManager::DrawStart)
{
}


MakeCutSceneManager::~MakeCutSceneManager(void)
{
}
