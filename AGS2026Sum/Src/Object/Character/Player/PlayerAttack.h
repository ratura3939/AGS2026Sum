#pragma once
#include "../../Common/ActorBase.h"
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

	//jsonから受け取る攻撃データ
    struct AttackData {
		int scale;      //大きさ
		int power;      //攻撃力
		int time;       //時間
		VECTOR localPos;   //ローカル座標
	};

    static const int ATTACK_LEVEL_MAX = 3;

	PlayerAttack(const VECTOR& _playerPos);
	~PlayerAttack(void)override;

	void Draw(void)override;
	void Release(void)override;

	void HitCollider(std::weak_ptr<Collider> _col)override;

	void Attack(const ATTACK_TYPE& _type);	//攻撃開始

private:
	void DoInit(void)override;
	void DoUpdate(void)override;

	void LoadAttackData(void);	//攻撃データの読み込み

	void ApplyAttackColliderSettings(void);	//コライダの設定

	const VECTOR& playerPos_;	//プレイヤーの座標参照
	AttackData data_[static_cast<int>(ATTACK_TYPE::MAX)][ATTACK_LEVEL_MAX];	//攻撃データ
	AttackData currentData_;	//現在の攻撃データ
	int level_;		//攻撃レベル
	int counter_;	//攻撃の時間管理
};

