#pragma once
#include <memory>
#include "../Common/ActorBase.h"
#include"../../Manager/GameSystem/AnimationController.h"

class CharacterBase :
    public ActorBase
{
public:
	//ノックバックの種類
	enum class KNOCKBACK_TYPE {
		STAGGER		//のけぞり
		,PUSH_BACK	//少し後退
		,LAUNCH		//打ち上げ
		,FLOAT		//浮き続ける
		,SLAM		//叩きつけ
		,BLOW_AWAY	//吹き飛び
		,MAX
	};

	//回転作業
	static constexpr float PER_ROT = 0.2f;			//フレームごとの回転(球面補間における時間の増加量を表す)
	static constexpr float THRESHOLD_ROT = 0.1f;	//回転のしきい値を表す

	static const std::string KNOCKBACK_TYPE_STRING[static_cast<int>(KNOCKBACK_TYPE::MAX)];	//ノックバックの種類の文字列配列

	CharacterBase(void);
	virtual ~CharacterBase(void)override;

	virtual void Draw(void)override = 0;			//描画
	virtual void Release(void)override = 0;			//解放

	virtual void HitCollider(std::weak_ptr<Collider> _col)override = 0;	//衝突後の処理

	void Damage(const float _damage);						//ダメージ処理
	const bool IsAlive(void)const { return hp_ > 0.0f; }	//生存判定

	const Quaternion& GetQua(void) const { return characterRotY_; }	//回転量の取得
	
	void SetAnimationSpeedPercent(const float _percent);	//アニメーション速度倍率設定

protected:
	virtual void DoInit(void)override = 0;		//初期化
	virtual void DoLoad(void)override = 0;		//読み込み
	virtual void DoUpdate(void)override = 0;	//更新

	virtual void InitAnim(void) = 0;			//アニメーションの初期化

	virtual void Move(void) = 0;				//移動処理
	virtual void Attack(void) = 0;				//攻撃処理
	virtual void Death(void);					//死亡処理

	void SetGoalRot(const float _rad);			//回転目標角度
	void Rotation(void);						//回転

	std::unique_ptr<AnimationController> animController_;	//アニメーションコントローラー

	float hp_;					//体力

private:

	Quaternion characterRotY_;	//Y軸回転用

	Quaternion goalQua_;		//目標の回転量
	float stepRotation_;		//回転のカウンター
};

