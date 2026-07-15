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
		,MAX
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
	/// エフェクトの再生
	/// </summary>
	/// <param name="_efc">エフェクト登録名</param>
	/// <param name="_pos">再生位置</param>
	/// <param name="_qua">角度</param>
	/// <param name="_size">大きさ</param>
	/// <param name="_sound">効果音</param>
	void Play(const std::wstring& _master, const EFFECT_NAME& _name,
		const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed = 1.0f,
		const SoundManager::SOUND_NAME& _sndName = SoundManager::SOUND_NAME::MAX);

	/// <summary>
	/// エフェクトの再生停止
	/// </summary>
	/// <param name="_efc">エフェクト登録名</param>
	void Stop(const std::wstring& _master,const EFFECT_NAME& _name);

	/// <summary>
	/// エフェクトの各パラメータ同期
	/// </summary>
	/// <param name="_efc">エフェクト名</param>
	/// <param name="_pos">位置情報</param>
	/// <param name="_qua">回転情報</param>
	/// <param name="_size">大きさ</param>
	void SyncEffect(const std::wstring& _master,const EFFECT_NAME& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size,const float& _speed,const int _idx);


	//更新処理
	void Update(void);

	//解放処理
	void Release(void);

	//消去処理
	void Destroy(void);

	//エフェクトが再生中かどうか
	const bool IsEffectPlay(const std::wstring& _master, const EFFECT_NAME& _name);
private:
	//インスタンス用
	static EffectManager* instance_;

	//エフェクトデータ格納用
	std::unordered_map<EFFECT_NAME,int> effectRes_;	//初期データ
	std::map<std::wstring,std::map<EFFECT_NAME,std::vector<int>>> effectPlay_;//再生データ

	//コンストラクタ＆デストラクタ
	EffectManager(void);
	~EffectManager() = default;
};

