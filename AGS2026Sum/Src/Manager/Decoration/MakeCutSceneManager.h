#pragma once
#include<memory>
#include"../../Common/Singleton.h"

class MakeCutSceneElementBase;
class StartUp;
class SelectFunction;
class MakeFile;
class EditData;
class AddNewData;

class MakeCutSceneManager
	: public Singleton<MakeCutSceneManager>
{
	friend class Singleton<MakeCutSceneManager>;

public:
	enum class FUNCTION {
		START_UP	//開始処理
		,SELECT		//機能選択
		,MAKE_FILE	//ファイルの生成
		,EDIT		//編集
		,ADD_DATA	//新規データ	
	};

	void Update(void);
	void DrawAtMainWindow(void);	//初期描画（元ウィンドウでの表示）
	void Draw(void);

	const bool IsUseMakeEdit(void)const { return useMakeCutSceneWindow_; }
	void ChangeFunction(const FUNCTION& _command);	//切り替え

private:
	/// <summary>
	/// ウィンドウの準備
	/// </summary>
	/// <param name="_showFlag">true=表示/false=非表示</param>
	void RedyWindow(const bool _showFlag);

	void MakeWindow(void);			//ウィンドウの生成

	MakeCutSceneManager(void);
	~MakeCutSceneManager(void)override;

	bool useMakeCutSceneWindow_;	//専用ウィンドウを使用しているか

	HWND editWindow_;
	int editScreen_;		//スクリーン

	std::weak_ptr<MakeCutSceneElementBase>useFunction_;

	std::shared_ptr<StartUp>startUp_;
	std::shared_ptr<SelectFunction>selectFunction_;
	std::shared_ptr<MakeFile>makeFile_;
	std::shared_ptr<EditData>editData_;
	std::shared_ptr<AddNewData>addNewData_;
};

