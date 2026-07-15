#pragma once
#include<memory>
#include"../../Common/Singleton.h"

class MakeCutSceneManager
	: public Singleton<MakeCutSceneManager>
{
	friend class Singleton<MakeCutSceneManager>;

public:
	enum class USER_INPUT_ACTION {
		NONE
		,SELECT		//機能選択
		,MAKE_FILE	//ファイルの生成
		,EDIT		//編集
		,ADD_DATA	//新規データ	
	};

	void Update(void);
	void DrawAtMainWindow(void);	//初期描画（元ウィンドウでの表示）
	void Draw(void);

	const bool IsUseMakeEdit(void)const { return useMakeCutSceneWindow_; }

private:
	void ChangeFunction(const USER_INPUT_ACTION& _command);	//切り替え

	void UpdateStart(void);				//開始時
	void UpdateSelectFunction(void);	//使用する機能選択
	void UpdateMakeFile(void);			//ファイル生成
	void UpdateEdit(void);				//既存データ編集
	void UpdateAddData(void);			//新規データ

	void DrawStart(void);			//開始時
	void DrawSelectFunction(void);	//使用する機能選択
	void DrawMakeFile(void);		//ファイル生成
	void DrawEdit(void);			//既存データ編集
	void DrawAddData(void);			//新規データ

	void MakeWindow(void);			//ウィンドウの生成

	MakeCutSceneManager(void);
	~MakeCutSceneManager(void)override;

	bool useMakeCutSceneWindow_;	//専用ウィンドウを使用しているか

	HWND editWindow_;
	int editScreen_;		//スクリーン

	using Func = void(MakeCutSceneManager::*)(void);

	Func updateFunc_;	//更新処理
	Func drawFunc_;		//描画処理

	
};

