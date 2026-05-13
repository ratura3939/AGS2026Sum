#pragma once
#include<string>
#include<memory>
#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Game;
class EnemyManager;
class PlayerChara;
class PlayerAttack;

class PlayerManager
{
public:
#pragma region アニメーション登録名
	static const std::wstring ANIM_IDLE;		//待機
	static const std::wstring ANIM_RUN;			//移動
	static const std::wstring ANIM_FIRST_PUNCH;	//初回パンチ
	static const std::wstring ANIM_SECOND_PUNCH;//二回目パンチ
	static const std::wstring ANIM_THIRD_PUNCH;	//三回目パンチ
	static const std::wstring ANIM_MIDDLE_KICK;	//中段キック(二段目を強派生)
	static const std::wstring ANIM_HIGH_KICK;	//上段キック(三段目を強派生)
	static const std::wstring ANIM_FINSH_KICK;	//最終段キック（パンチ三回目後、強派生）
#pragma endregion

	PlayerManager(Game& _gameScene);
	~PlayerManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//位置・回転取得
	const VECTOR& GetPos(void)const;		//座標
	const Quaternion& GetQua(void);			//回転
	//const VECTOR& GetFocusPoint(void);	//注視点

	//const bool IsAlive(void)const;

private:
	void UserInput(void);				//入力受付

	Game& scene_;	//ゲームクラス参照

	std::shared_ptr<PlayerChara> character_;	//キャラクター
	std::unique_ptr<PlayerAttack> attack_;		//攻撃
};

