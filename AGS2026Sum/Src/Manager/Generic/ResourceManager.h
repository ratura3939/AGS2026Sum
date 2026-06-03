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
		PLAYER_MDL	//プレイヤー
		,ENEMY_MDL	//敵
		,BOSS_MDL	//ボス

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

		//敵
		, ENEMY_IDLE_ANIM			//待機
		, ENEMY_WALK_ANIM			//歩く
		, ENEMY_RUN_ANIM			//走る
		, ENEMY_ATTACK_ANIM			//攻撃
		, ENEMY_BLOW_ANIM			//吹っ飛び
		, ENEMY_DEATH_ANIM			//死亡


		//JSON
		,PLAYER_ATTACK_DATA_JSON	//プレイヤー攻撃データ

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

