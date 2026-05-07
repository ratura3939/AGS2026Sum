#pragma once
#include<string>
#include<vector>
#include<unordered_map>

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

	//アニメーション関連情報
	struct AnimationInfo {
		PLAY_TYPE type;		//再生タイプ
		ANIM_SOURCE source; //アニメーションの情報源
		int data;			//EMBEDDEDならアニメーション番号、EXTERNALならリソースID
		float total;		//総再生時間
		bool mustPlayOnce;	//再生保障
	};

	//アタッチに関する情報
	struct AttachInfo {
		int attachNum;
		float speed;
		float counter;
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
	void Add(const std::wstring& _name, const int _animData, const PLAY_TYPE& _type, const ANIM_SOURCE& _source, const bool _isLock = false);

	/// <summary>
	/// 再生開始処理
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_speed">再生速度</param>
	/// <param name="_next">連続して再生する物たち<最後以外にLOOPのものを入れないこと！！></param>
	void Play(const std::wstring& _name, const float _speed = DEFAULT_SPEED, const std::vector<std::wstring> _next = {});

	/// <summary>
	/// 連続して再生するアニメーションを途中で追加する
	/// </summary>
	/// <param name="_name">追加するアニメーション名</param>
	void AddNextAnim(const std::wstring& _name);

	/// <summary>
	/// 連続して再生するアニメーションを途中で追加する
	/// </summary>
	/// <param name="_name">追加するアニメーション名(複数)</param>
	void AddNextAnim(const std::vector<std::wstring> _names);

	//更新
	void Update(void);

	/// <summary>
	/// 再生速度(割合)を設定
	/// </summary>
	/// <param name="_percent">0％～100％</param>
	void ChangeSpeedRate(const float _percent);

	void UnAnimLock(void) { isAnimLock_ = false; }
	void SetDefaultAnim(const std::wstring& _name);

private:
	//アニメーション更新処理
	void UpdateNomalAnim(void);
	void UpdateReturnAnim(void);

	//アニメーション終了時処理
	void FinishAnimNomal(void);
	void FinishAnimLoop(void);
	void FinishAnimReturn(void);

	//アニメーション再生情報の設定
	void SetAnimationPlayInfo(AnimationInfo& _animInfo, const AnimationInfo& _sourceInfo, AttachInfo& _attachInfo, const float _animSpeed);

	//アニメーション終了時と更新処理の設定
	void SetFinishAndUpdateFunc(void);

	//アニメーションブレンド処理
	void BlendAnim(void);
	void FinishBlendAnim(void);

	//アニメーションのアタッチ処理
	void SetAttachAnim(int& _attachAnim,const int _animData,const ANIM_SOURCE& _source);

	int& modelId_;	//モデルID
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

	std::vector<std::wstring> nextAnimList_;	//次に再生するアニメーション(LOOP以外に適用)<最終以外にLOOＰを入れないこと>

	using FinishAnimation = void(AnimationController::*)(void);
	using UpdateAnimation = void(AnimationController::*)(void);
	FinishAnimation finishAnim_;	//終了時処理関数ポインタ
	UpdateAnimation updateAnim_;	//更新処理関数ポインタ
};

