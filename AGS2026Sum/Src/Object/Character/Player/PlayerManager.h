#pragma once
#include<string>
#include<memory>
#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Game;
class EnemyManager;
class PlayerChara;

class PlayerManager
{
public:
	PlayerManager(Game& _gameScene);
	~PlayerManager(void);

	void Init(void);
	void Update(AttackManager& _atk);
	void Draw(void);
	void Release(void);

	//位置・回転取得
	const VECTOR& GetPos(void)const;		//座標
	//const Quaternion& GetQua(void);		//回転
	//const VECTOR& GetFocusPoint(void);	//注視点

	//const bool IsAlive(void)const;

	//void DrawDebug(void);

private:
	void UserInput(void);				//入力受付

	Game& scene_;	//ゲームクラス参照

	std::shared_ptr<PlayerChara> character_;//キャラクター
};

