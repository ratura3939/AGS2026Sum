#pragma once
#include "../CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:

	//コンストラクタ
	EnemyBase(const int _num);

	//デストラクタ
	~EnemyBase(void);

	//読み込み
	void Load(void)override;

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//衝突後の処理
	void HitCollider(std::weak_ptr<Collider> _col)override;

private:

	//当たり判定
	static constexpr float RADIUS = 30.0f;
	static constexpr float BROUD_RADIUS = RADIUS + 20.0f;

	//自分の番号
	const int myNum_;

	//初期化
	void DoInit(void)override;

	//更新
	void DoUpdate(void)override;

	//描画(デバッグ)
	void DrawDebug(void);

	//移動処理
	void Move(void)override;

	//攻撃処理
	void Attack(void)override;
};

