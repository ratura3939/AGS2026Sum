#pragma once
#include<string>

//敵の種類
enum class ENEMY_TYPE
{
	NORMAL,			//雑魚
	MIDDLE_BOSS,	//中ボス
	//BOSS,			//大ボス
	MAX
};

//String型のキャスト
static const ENEMY_TYPE& CastStringToEnemyType(std::string _string)
{
	if (_string == "NormalEnemy") {
		return ENEMY_TYPE::NORMAL;
	}
	else if (_string == "MiddleBoss") {
		return ENEMY_TYPE::MIDDLE_BOSS;
	}
	//else if (_string == "Boss") return ENEMY_TYPE::BOSS;
	else return ENEMY_TYPE::NORMAL;
}

//グループごとの状態
enum class GROUP_ORDER
{
	STAY				//待機
	, MOVE				//移動
	, ALERT				//警戒
	, MAX
};

//敵個々の行動
enum class ENEMY_ACTION
{
	STAY				//待機
	, MOVE				//移動
	, ALERT				//警戒
	, ATTACK_READY		//攻撃準備
	, ATTACK			//攻撃
	, ATTACK_END		//攻撃終了
	, RETURN_GROUP		//グループに戻る
	, MAX
};

//敵の状態
enum class ENEMY_STATE
{
	NORMAL				//通常
	, STAGGER			//よろけ
	, LAUNTH			//打ち上げ
	, SLAM				//叩きつけ
	, PUSH_BACK			//小吹っ飛び
	, BLOW_AWAY			//大吹っ飛び
	, KNOCK_DOWN		//ダウン
	, DEATH				//死亡
	, FADE				//フェードアウト
	, END				//終了
	, MAX
};