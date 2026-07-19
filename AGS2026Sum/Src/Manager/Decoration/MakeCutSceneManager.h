#pragma once
#include<memory>
#include"../../Common/Singleton.h"

class MakeCutSceneElementBase;
class StartUp;
class EditDataScene;

class MakeCutSceneManager
	: public Singleton<MakeCutSceneManager>
{
	friend class Singleton<MakeCutSceneManager>;

public:
	enum class FUNCTION {
		START_UP	//開始処理
		,EDIT		//編集
	};

	void Init(void)override;
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
	std::shared_ptr<EditDataScene>editData_;
};

