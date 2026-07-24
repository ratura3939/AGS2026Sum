#pragma once
#include<unordered_map>


//Dxlib内に音声データを3D空間上に再生する処理がある
//簡単に言えば音声に距離をつけることが可能
//少しばかり複雑そうなので後日に回す


class SoundManager
{
public:
	//登録名
	enum class SOUND_NAME {
		//BGM
		TITLE_BGM
		,GAME_BGM

		//SE
		//システム
		,ENTER_SE
		,MOVE_CUSUR_SE

		//プレイヤー攻撃
		,PUNCH_FIRST_PLAYER_SE
		,PUNCH_SECOND_PLAYER_SE
		,PUNCH_THIRD_PLAYER_SE
		,KICK_FIRST_PLAYER_SE
		,KICK_SECOND_PLAYER_SE
		,KICK_THIRD_PLAYER_SE
		,PUNCH_SPECIAL_PLAYER_SE
		,KICK_SPECIAL_PLAYER_SE
		,ULTIMATE_REDY
		,ULTIMATE

		//敵
		, ENEMY_CHARGE_SE			
		, ENEMY_NORMAL_SKILL_SE		
		, ENEMY_TACKLE_SKILL_SE		
		, ENEMY_JUMP_SKILL_SE		
		, ENEMY_SKILL_CANCEL_SE		
		, ENEMY_GUARD_BREAK_SE		

		,MAX
	};

	//再生の種類分けするときに使う
	enum class TYPE {
		NONE
		,BGM
		,SE
		,MAX
	};

	struct SOUND_DATA
	{
		int data;		//音声データ格納
		TYPE type;		//再生種類
		int playMode;	//音声データの再生タイプ
	};

	static constexpr int VOLUME_BASIC = 255 / 100;

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SoundManager& GetInstance(void);

	/// <summary>
	/// サウンドの追加
	/// </summary>
	/// <param name="_type">音の種類分け(SEかBGMか)</param>
	/// <param name="_name">登録名</param>
	/// <param name="_data">音のデータ</param>
	/// <param name="_interval">再生間隔制限</param>
	void Add(const TYPE& _type, const SOUND_NAME& _name, const int _data, const int _interval = 0);

	/// <summary>
	/// 音声データ
	/// </summary>
	/// <param name="_name">登録名</param>
	void Play(const SOUND_NAME& _name);

	/// <summary>
	/// 停止処理
	/// </summary>
	/// <param name="_name">登録名</param>
	void StopAll(const SOUND_NAME& _name);

	//更新
	void Update(void);

	//解放
	void Release(void);

	/// <summary>
	/// 音量調節
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_persent">調整割合(0%～100%)</param>
	void AdjustVolume(const SOUND_NAME& _name, const int _persent);
	/// <summary>
	/// 音量調節
	/// </summary>
	/// <param name="_playType">調節対象(BGMorSE)</param>
	/// <param name="_persent">調整割合(0%～100%)</param>
	void AdjustVolume(const TYPE& _playType, const int _persent);

	/// <summary>
	/// ピッチの調整(音の高さを設定する場合Addの前にこの処理を書くこと)
	/// 使用後、調整の必要がなくなったら引数なしで呼び出すこと
	/// </summary>
	/// <param name="_rate">100.0f=半音高く、1200.0f=１オクターブ高く(低くする場合は-を付ける)</param>
	void AdjustPitchRate(const float _rate = 0.0f)const;

	/// <summary>
	/// 再生速度の調整(再生速度を設定する場合Addの前にこの処理を書くこと)
	/// 使用後、調整の必要がなくなったら引数なしで呼び出すこと
	/// </summary>
	/// <param name="_rate">倍率</param>
	void AdjustTimeRate(const float _rate = 1.0f)const;

	void Destroy(void);

	void NoSound(void) { isNoPlaySound_ = true; }

private:
	//インスタンス用
	static SoundManager* instance_;

	//データ格納用
	std::unordered_map<SOUND_NAME, SOUND_DATA> sounds_;
	SOUND_NAME activeBgm_;

	//再生間隔
	std::unordered_map<SOUND_NAME,int> intervales_;
	std::unordered_map<SOUND_NAME,int> counteres_;

	bool isNoPlaySound_ = false;	//デバッグ用：再生不可フラグ

	//コンストラクタ＆デストラクタ
	SoundManager() = default;
	~SoundManager() = default;
};

