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
    };

    static const int ATTACK_LEVEL_MAX = 3;

	PlayerAttack(const VECTOR& _playerPos, const Quaternion& _playerQuaRot);
	~PlayerAttack(void)override;

	void Draw(void)override;
	void Release(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

	//攻撃開始
	void Attack(const ATTACK_TYPE& _type);	
	//現在の攻撃アニメーション登録名の取得
	const std::wstring& GetCurrentAttackAnimName(void)const;	
	//攻撃中か
	const bool IsAttacking(void)const;

private:
	void DoLoad(void)override;
	void DoInit(void)override;
	void DoUpdate(void)override;

	//攻撃データの読み込み
	void LoadAttackData(void);	
	//コライダの設定
	void ApplyAttackColliderSettings(void);

	const VECTOR& playerPos_;	//プレイヤーの座標参照
	const Quaternion& playerQuaRot_;	//プレイヤーの回転参照

	AttackData data_[static_cast<int>(ATTACK_TYPE::MAX)][ATTACK_LEVEL_MAX];	//攻撃データ
	std::wstring animNames_[static_cast<int>(ATTACK_TYPE::MAX)][ATTACK_LEVEL_MAX];	//攻撃アニメーション登録名

	AttackData currentData_;	//現在の攻撃データ
	ATTACK_TYPE currentType_;	//現在の攻撃種別
	int level_;		//攻撃レベル
	int counter_;	//攻撃の時間管理

	int debugColor_;	//デバッグ用の色
};

