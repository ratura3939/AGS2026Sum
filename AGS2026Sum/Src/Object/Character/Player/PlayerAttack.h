#pragma once
#include "../../Common/ActorBase.h"
#include"../../../Manager/Decoration/SoundManager.h"
#include "PlayerAttackData.h"

class PlayerAttack :
    public ActorBase
{
public:
    //攻撃種別
    enum class ATTACK_TYPE {
		PUNCH   //パンチ
		,KICK   //キック
		,SPECIAL //特殊攻撃
        ,MAX
    };

	struct AttackAnimationInfo {
		std::wstring name;	//アニメーション登録名
		float speed;		//アニメーションの再生速度
	};

	struct ComboRouteInfo {
		ATTACK_TYPE type;	//攻撃種別
		bool isUsed;		//攻撃を行ったか
		bool isDrawed;		//描画したか
	};

	struct AttackSeInfo {
		SoundManager::SOUND_NAME seName;
		float timing;
	};

	static const int ATTACK_LEVEL_MAX = 3;				//攻撃レベルの最大値
	static constexpr float ATTACK_CANCEL_RATE = 0.5f;	//攻撃キャンセル可能割合(アニメーションの進行度)

	PlayerAttack(const VECTOR& _playerPos, const Quaternion& _playerQuaRot);
	~PlayerAttack(void)override;

	void Draw(void)override;
	void Release(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

	
	const bool ReserveAttack(const ATTACK_TYPE& _type);	//攻撃の予約
	void Attack(void);			//攻撃開始
	void FinishAttack(void);	//攻撃終了処理

	const AttackAnimationInfo GetNextAttackAnimInfo(void)const;		//次の攻撃アニメーション情報の取得
	const AttackSeInfo GetNextAttackSeInfo(void)const;				//次の攻撃のSEに関する情報の取得

	const std::string& GetCurrentAttackKnockBackType(void)const;	//現在の攻撃のノックバックの種類の取得
	
	const bool IsAttacking(void)const;	//攻撃中か

	//デバッグ用
	void DrawDebug(void);

private:
	void DoLoad(void)override;
	void DoInit(void)override;
	void DoUpdate(void)override;

	void DrawComboRoute(void);	//コンボルートの描画
	void DrawComboRouteElement(const std::string& _attackKey, const VECTOR& _pos);	//コンボルートの要素の描画

	void LoadAttackData(void);				//攻撃データの読み込み
	void LoadAttackSound(void);				//効果音などの読み込み
	void ApplyAttackColliderSettings(void);	//コライダの設定
	void ResetCombo(void);					//コンボのリセット
	void ResetComboRoute(void);				//コンボルートのリセット

	const VECTOR& playerPos_;	//プレイヤーの座標参照
	const Quaternion& playerQuaRot_;	//プレイヤーの回転参照

	std::unordered_map<std::string, AttackData> data_;			//攻撃データ
	std::unordered_map<std::string, std::wstring> animNames_;	//攻撃アニメーション登録名
	std::unordered_map<std::string, ComboRouteInfo> comboRouteInfos_;		//コンボルート情報
	std::unordered_map<std::string, SoundManager::SOUND_NAME> seNames_;	//攻撃効果音登録名

	AttackData currentData_;	//現在の攻撃データ

	std::string currentAttackName_;	//現在の攻撃アニメーション登録名
	std::string nextAttackName_;		//次の攻撃アニメーション登録名

	ATTACK_TYPE latestReserveType_;		//最新に予約された攻撃種別

	bool comboReset_;	//コンボリセットフラグ
	int comboResetCounter_;	//コンボリセットカウンタ

	std::string startAttackAnimName_;	//攻撃開始アニメーション登録名

	int debugColor_;	//デバッグ用の色
};

