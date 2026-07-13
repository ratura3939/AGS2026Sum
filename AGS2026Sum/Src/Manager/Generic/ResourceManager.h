#pragma once
#include <map>
#include <string>
#include "Resource.h"
#include"SceneManager.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		//モデル
		PLAYER_MDL			//プレイヤー
		,ENEMY_MDL			//敵
		,MIDDLE_BOSS_MDL	//ボス

		//画像
		,TITLE_BACK_IMG			//タイトル画面
		,GAME_CLEAR_BACK_IMG	//ゲームクリア画面
		,GAME_OVER_BACK_IMG		//ゲームオーバー画面
		,START_BUTTON_IMG		//スタートボタン
		,END_BUTTON_IMG			//終了ボタン
		,BACK_TITLE_IMG			//タイトルに戻る
		,BACK_GAME_IMG			//ゲームに戻る
		,COMBO_NUMBER_IMG		//コンボ用の数字
		,COMBO_STRING_IMG		//「COMBO」画像

		//アニメーション
		//プレイヤー
		,PLAYER_IDLE_ANIM			//待機
		,PLAYER_RUN_ANIM			//走る
		,PLAYER_DAMAGE_ANIM			//ダメージ
		,PLAYER_FIRST_PUNCH_ANIM	//パンチ１
		,PLAYER_SECOND_PUNCH_ANIM	//パンチ２
		,PLAYER_THIRD_PUNCH_ANIM	//パンチ３
		,PLAYER_MIDDLE_KICK_ANIM	//キック１
		,PLAYER_HIGH_KICK_ANIM		//キック２
		,PLAYER_FINISH_KICK_ANIM	//キック３
		,PLAYER_SPECIAL_PUNCH_ANIM	//特殊攻撃(パンチ派生)
		,PLAYER_SPECIAL_KICK_ANIM	//特殊攻撃(キック派生)
		,PLAYER_ULTIMET_ANIM		//必殺技
		,PLAYER_ULTIMET_TEST_ANIM	//必殺技(テスト用)

		//敵
		, ENEMY_IDLE_ANIM				//待機
		, ENEMY_WALK_ANIM				//歩く
		, ENEMY_RUN_ANIM				//走る
		, ENEMY_ATTACK_ANIM				//攻撃
		, ENEMY_TACKLE_ANIM				//突進
		, ENEMY_JUMP_ANIM				//ジャンプ
		, ENEMY_STAGGER_ANIM			//よろけ
		, ENEMY_LAUNCH_ANIM				//打ち上げ
		, ENEMY_LAUNCH_DOWN_ANIM		//打ち上げ倒れ
		, ENEMY_BLOW_FIRST_HALF_ANIM	//吹っ飛び前半
		, ENEMY_BLOW_SECOND_HALF_ANIM	//吹っ飛び後半
		, ENEMY_BLOW_END_ANIM			//吹っ飛び終了
		, ENEMY_DEATH_ANIM				//死亡

		//ステージ
		,FIRST_STAGE_MDL	//初期位置モデル
		,BOSS_STAGE_MDL		//ボスステージ
		,DOOR_RIGHT_MDL		//ドア右
		,DOOR_LEFT_MDL		//ドア左

		//エフェクト
		,PLAYER_PUNCH_THIRD_EFC		//パンチ三段目
		,PLAYER_KICK_THIRD_EFC		//キック三段目
		,PLAYER_PUNCH_SPECIAL_EFC	//パンチ特殊
		,PLAYER_ULTIMATE_EFC		//必殺技エフェクト

		//サウンド
		// BGM
		,GAME_BGM		//ゲームシーン

		//SE
		,MOVE_CURSUR_SE
		,ENTER_SE

		,PUNCH_FIRST_PLAYER_SE		//パンチ初段
		,PUNCH_SECOND_PLAYER_SE		//パンチ二段目
		,PUNCH_THIRD_PLAYER_SE		//パンチ最終段
		,KICK_FIRST_PLAYER_SE		//キック初段
		,KICK_SECOND_PLAYER_SE		//キック二段目
		,KICK_THIRD_PLAYER_SE		//キック最終段

		//JSON
		,TITLE_UI_INFO_JSON			//タイトルUI
		,PLAYER_ATTACK_DATA_JSON	//プレイヤー攻撃データ
		,PLAYER_ATTACK_DIRECTION_DATA_JSON	//攻撃演出
		,NORMAL_ENEMY_PARAMETER		//雑魚敵パラメーター
		,MIDDLE_BOSS_PARAMETER		//中ボスパラメーター
		,ENEMY_SKILLS_DATA			//敵スキルのデータ
		,ENEMY_ANIMATION_DATA		//敵のアニメーションデータ

		,MAX
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(SceneManager::SCENE_ID _scene= SceneManager::SCENE_ID::NONE);
	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:
	//シーンごとにデータを読み込むことにする
	void InitTitle(void);
	void InitGame(void);
	void InitClear(void);
	void InitGameOver(void);
	void InitPause(void);

	void ResourcePlayer(void);	//プレイヤー関連リソース
	void ResourceEnemy(void);	//敵関連リソース
	void ResourceBoss(void);	//ボス関連リソース
	void ResourceCursurSound(void);	//ボタンおよびカーソルに関する音声

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;
};

