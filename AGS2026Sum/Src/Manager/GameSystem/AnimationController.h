#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include"../Decoration/SoundManager.h"

class AnimationController
{
public:
	static constexpr float DEFAULT_SPEED = 1.0f;
	static constexpr float DEFAULT_SPEED_RATE = 100.0f;

	/// <summary>
	/// アニメーションの情報源
	/// </summary>
	enum class ANIM_SOURCE {
		EMBEDDED	//埋め込み
		,EXTERNAL	//外部
		,MAX
	};

	/// <summary>
	/// 再生種類
	/// </summary>
	enum class PLAY_TYPE {
		NORMAL,	//通常再生
		LOOP,	//ループ再生
		RETURN,	//逆再生
		MAX
	};

	//アニメーションに合わせて再生するSEの情報
	struct AnimationSoundInfo {
		SoundManager::SOUND_NAME name;
		float playTiming;	//再生するタイミング(アニメーションの再生時間に対する割合)
		bool isPlayed;		//すでに再生したかどうか
	};

	//アニメーション関連情報
	struct AnimationInfo {
		std::wstring name;	//登録名
		PLAY_TYPE type;		//再生タイプ
		ANIM_SOURCE source; //アニメーションの情報源
		int data;			//EMBEDDEDならアニメーション番号、EXTERNALならリソースID
		float total;		//総再生時間
		bool mustPlayOnce;	//再生保障
		bool isFixPosition;	//位置補正を行うか
		AnimationSoundInfo seInfo;	//アニメーションに合わせて再生するSEの情報
	};

	//アタッチに関する情報
	struct AttachInfo {
		int attachNum;
		float speed;
		float counter;
		bool isFinish;
	};

	//予約アニメーション情報
	struct NextAnimInfo {
		std::wstring name;	//アニメーション名
		float speed;		//再生速度
	};

	//補正を行う軸
	struct FixAnimationAxis {
		bool x;
		bool y;
		bool z;
	};

	AnimationController(int& _model);
	~AnimationController(void);

	/// <summary>
	/// アニメーション追加関数
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_animData">アニメーション番号</param>
	/// <param name="_type">再生タイプ</param>
	/// <param name="_source">アニメーションの情報源</param>
	/// <param name="_isLock">一回の再生を保障するかどうか</param>
	/// <param name="_isFixPosition">アニメーションによる移動を補正するかどうか</param>
	void Add(const std::wstring& _name, const int _animData, const PLAY_TYPE& _type, const ANIM_SOURCE& _source, const bool _isLock = false, const bool _isFixPosition = false);

	/// <summary>
	/// 再生開始処理
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_speed">再生速度</param>
	/// <param name="_next">連続して再生する物たち<最後以外にLOOPのものを入れないこと！！></param>
	void Play(const std::wstring& _name, const float _speed = DEFAULT_SPEED, const AnimationSoundInfo& _seInfo = { SoundManager::SOUND_NAME::MAX, 0.0f, true }, const std::vector<NextAnimInfo> _next = {});

	/// <summary>
	/// 強制再生処理
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_speed">再生速度</param>
	/// <param name="_next">連続して再生する物たち<最後以外にLOOPのものを入れないこと！！></param>
	void ForcePlay(const std::wstring& _name, const float _speed = DEFAULT_SPEED, const AnimationSoundInfo& _seInfo = { SoundManager::SOUND_NAME::MAX, 0.0f, true }, const std::vector<NextAnimInfo> _next = {});

	/// <summary>
	/// 連続して再生するアニメーションを途中で追加する
	/// </summary>
	/// <param name="_name">追加するアニメーション名</param>
	void AddNextAnim(const std::wstring& _name, const float _speed);

	/// <summary>
	/// 連続して再生するアニメーションを途中で追加する
	/// </summary>
	/// <param name="_name">追加するアニメーション名(複数)</param>
	void AddNextAnim(const std::vector<NextAnimInfo> _animations);

	//更新
	void Update(void);

	/// <summary>
	/// 再生速度(割合)を設定
	/// </summary>
	/// <param name="_percent">0％～100％</param>
	void ChangeSpeedRate(const float _percent);

	void UnAnimLock(void) { isAnimLock_ = false; }		//アニメーションロック解除
	void SetDefaultAnim(const std::wstring& _name);		//デフォルトアニメーションの設定

	//現在のアニメーションの再生進行度合いを割合で取得
	const float GetCurrentAnimationProgressRate(void)const;
	const float GetBlendAnimationProgressRate(void)const;

	const bool IsFinishNormalAnim(void)const { return isFinishNormalAnim_; }	//今のアニメーションの再生が終了したかどうか
	const bool IsStartNextAnim(void)const { return isStartNextAnim_; }			//次のアニメーションの再生が開始されたかどうか

	const float GetAnimTotalTime(const std::wstring& _name)const;	//アニメーションの総再生時間を取得

	void SetRootFrameIndex(const std::wstring& _frameName);	//モデルの親フレームの設定
	void SetFixAnimationAxisInfo(const std::wstring _name, const bool _x, const bool _y, const bool _z);	//位置補正を行う軸の設定

	//デバッグ用
	void DrawNextAnimations(void);

private:
	//アニメーション更新処理
	void UpdateNormalAnim(void);
	void UpdateReturnAnim(void);

	//アニメーション終了時処理
	void FinishAnimNormal(void);
	void FinishAnimLoop(void);
	void FinishAnimReturn(void);

	//アニメーション再生情報の設定
	void SetAnimationPlayInfo(AnimationInfo& _animInfo, const AnimationInfo& _sourceInfo, AttachInfo& _attachInfo, const float _animSpeed, const AnimationSoundInfo& _seInfo);

	//アニメーション終了時と更新処理の設定
	void SetFinishAndUpdateFunc(void);

	//アニメーションブレンド処理
	void BlendAnim(void);
	void FinishBlendAnim(void);

	//アニメーションのアタッチ処理
	void SetAttachAnim(int& _attachAnim,const int _animData,const ANIM_SOURCE& _source);

	//位置補正
	void FixPosition();
	//位置補正を行う軸を取得
	const FixAnimationAxis GetUseFixAnimationAxisData(void)const;

	int& modelId_;	//モデルID
	int rootFrameIdx_;	//モデルの親フレーム

	std::unordered_map<std::wstring, AnimationInfo>animDatas_;	//アニメーションデータ総まとめ
	AnimationInfo currentAnim_;				//再生中のアニメーション情報
	AnimationInfo blendAnim_;			//次に再生するアニメーション情報(ブレンド先)

	AttachInfo currentAnimAttachInfo_;	//再生中のアニメーションアッタッチ情報
	AttachInfo blendAnimAttachInfo_;	//ブレンドのアニメーションアッタッチ情報

	float speedRate_;		//速度割合

	bool isAnimLock_;		//アニメーションロック中かどうか
	float animBlendRate_;	//アニメーションブレンド率(0.0f~1.0f)	

	bool isSetDefaultAnim_;		//デフォルトアニメーションが設定されているかどうか
	std::wstring defaultAnim_;	//デフォルトアニメーションの名前

	std::vector<NextAnimInfo> nextAnimList_;	//次に再生するアニメーション(LOOP以外に適用)<最終以外にLOOＰを入れないこと>

	using FinishAnimation = void(AnimationController::*)(void);
	using UpdateAnimation = void(AnimationController::*)(void);
	FinishAnimation finishAnim_;	//終了時処理関数ポインタ
	UpdateAnimation updateAnim_;	//更新処理関数ポインタ

	bool isFinishNormalAnim_;	//今のアニメーションの再生が終了したかどうか
	bool isStartNextAnim_;		//次のアニメーションの再生が開始されたかどうか

	bool isUseFixPositionMethod_;	//位置補正用の処理を行ったか
	std::unordered_map<std::wstring, FixAnimationAxis>fixAxisData_;
};

