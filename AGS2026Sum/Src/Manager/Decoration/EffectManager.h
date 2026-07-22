#pragma once
#include<DxLib.h>
#include"SoundManager.h"
#include"../../Common/Quaternion.h"
#include<unordered_map>
#include<map>
#include<string>

//無駄のないように要改善

class EffectManager
{
public:
	//エフェクト登録名
	enum class EFFECT_NAME {
		EXPLOSION
		,PLAYER_PUNCH_THIRD		//パンチ三段目
		,PLAYER_KICK_THIRD		//キック三段目
		,PLAYER_PUNCH_SPECIAL	//パンチ特殊
		,PLAYER_ULTIMATE		//必殺技
		,PLAYER_HIT				//プレイヤーヒット
		,ENEMY_HIT				//敵ヒット
		,ENEMY_DEAD				//死亡
		,ENEMY_TACKLE			//突進
		,ENEMY_LANDING			//ジャンプ着地
		,MAX
	};

	//再生記録
	struct PlayRecord {
		EFFECT_NAME name;	//再生エフェクト
		int playHndle;		//再生データ
	};

	//各種エフェクトの上限
	static constexpr int NONE_MAX = 5;

	static void CreateInstance(void);

	// インスタンスの取得
	static EffectManager& GetInstance(void);

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="_name">エフェクト登録名</param>
	/// <param name="_data">エフェクトのデータ</param>
	void Add(const EFFECT_NAME& _name,int _data);

	/// <summary>
	/// エフェクトの再生(返り値を必ず保存してください)
	/// </summary>
	/// <param name="_efc">エフェクト登録名</param>
	/// <param name="_pos">再生位置</param>
	/// <param name="_qua">角度</param>
	/// <param name="_size">大きさ</param>
	/// <param name="_sound">効果音</param>
	/// <returns>再生ハンドル</returns>
	/// [nodiscard]付与予定
	int Play(const std::wstring& _master, const EFFECT_NAME& _name,
		const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed = 1.0f,
		const SoundManager::SOUND_NAME& _sndName = SoundManager::SOUND_NAME::MAX);

	/// <summary>
	/// エフェクトの再生停止(単一)
	/// </summary>
	/// <param name="_playHandle"></param>
	void StopOrderPlayHandle(const int _playHandle);

	/// <summary>
	/// エフェクトの再生停止(種類別)
	/// </summary>
	/// <param name="_name"></param>
	void StopOrderEffectName(const EFFECT_NAME& _name);

	/// <summary>
	/// エフェクトの再生停止(すべて)
	/// </summary>
	/// <param name="_efc">エフェクト登録名</param>
	void StopAll(void);

	/// <summary>
	/// エフェクトの各パラメータ同期(falseの場合は所持している_playerHandleを削除してください)
	/// </summary>
	/// <param name="_efc">エフェクト名</param>
	/// <param name="_pos">位置情報</param>
	/// <param name="_qua">回転情報</param>
	/// <param name="_size">大きさ</param>
	/// <returns>true=正常に行われた/false=データが存在しないため同期不可</returns>
	bool SyncEffect(const int _playHandle, const VECTOR& _pos, const Quaternion& _qua, const float& _size,const float& _speed);


	//更新処理
	void Update(void);

	//解放処理
	void Release(void);

	//消去処理
	void Destroy(void);

	//エフェクトが再生中かどうか
	const bool IsEffectPlay(const int _playHandle);
private:
	//インスタンス用
	static EffectManager* instance_;

	//エフェクトデータ格納用
	std::unordered_map<EFFECT_NAME,int> effectRes_;	//初期データ
	std::vector<PlayRecord> effectPlay_;	//再生データ

	//コンストラクタ＆デストラクタ
	EffectManager(void);
	~EffectManager() = default;
};

