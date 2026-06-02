#pragma once
#include "../../Common/ActorBase.h"
#include "PlayerAttackData.h"

class PlayerAttack :
    public ActorBase
{
public:
    //攻撃種別
    enum class ATTACK_TYPE {
		PUNCH   //パンチ
		,KICK   //キック
        ,MAX
		,DEBUG
    };

	struct AttackAnimationInfo {
		std::wstring name;	//アニメーション登録名
		float speed;		//アニメーションの再生速度
	};

	static const int ATTACK_LEVEL_MAX = 3;				//攻撃レベルの最大値
	static constexpr float ATTACK_CANCEL_RATE = 0.6f;	//攻撃キャンセル可能割合(アニメーションの進行度)

	PlayerAttack(const VECTOR& _playerPos, const Quaternion& _playerQuaRot);
	~PlayerAttack(void)override;

	void Draw(void)override;
	void Release(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

	//攻撃の予約
	const bool ReserveAttack(const ATTACK_TYPE& _type);

	//攻撃開始
	void Attack(void);	
	//現在の攻撃アニメーション情報の取得
	const AttackAnimationInfo GetNextAttackAnimInfo(void)const;
	//攻撃中か
	const bool IsAttacking(void)const;

	void FinishAttack(void);	//攻撃終了処理

	void SetCurrentAttackTotalTime(const float _totalTime, const float _speedRate) { currentData_.time = static_cast<int>(_totalTime * _speedRate); }	//現在の攻撃の総時間の設定(主にアニメーションの総再生時間)


	//デバッグ用
	void DrawDebug(void);

private:
	void DoLoad(void)override;
	void DoInit(void)override;
	void DoUpdate(void)override;

	//攻撃データの読み込み
	void LoadAttackData(void);	
	//コライダの設定
	void ApplyAttackColliderSettings(void);

	void CorrectionAttackLevel(const ATTACK_TYPE& _type);	//攻撃レベルの補正
	void ResetAttackLevel(void);	//攻撃のリセット

	const VECTOR& playerPos_;	//プレイヤーの座標参照
	const Quaternion& playerQuaRot_;	//プレイヤーの回転参照

	std::unordered_map<std::string, AttackData> data_;	//攻撃データ
	std::unordered_map < std::string,std::wstring> animNames_;	//攻撃アニメーション登録名

	AttackData currentData_;	//現在の攻撃データ
	ATTACK_TYPE currentType_;	//現在の攻撃種別
	ATTACK_TYPE nextType_;		//次の攻撃種別

	std::string currentAttackName_;	//現在の攻撃アニメーション登録名
	std::string nextAttackName_;		//次の攻撃アニメーション登録名

	ATTACK_TYPE latestReserveType_;		//最新に予約された攻撃種別
	int level_;		//攻撃レベル
	bool isKickCorrection_;	//キックのレベル補正中か

	int debugColor_;	//デバッグ用の色
};

