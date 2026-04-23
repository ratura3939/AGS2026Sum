#pragma once
#include "../CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:

	//コンストラクタ
	EnemyBase(const VECTOR& _groupPos, const int _num);

	//デストラクタ
	~EnemyBase(void);

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//衝突後の処理
	void HitCollider(std::weak_ptr<Collider> _col)override;

private:

	//当たり判定
	static constexpr float RADIUS = 30.0f;
	static constexpr float BROUD_RADIUS = RADIUS + 15.0f;

	//敵の行動間隔
	static constexpr float ACTION_INTERVAL = 2.0f;

	//グループ座標
	const VECTOR& groupPos_;

	//自分の番号
	const int myNum_;

	//行動間隔タイマー
	float actionTimer_;

	//読み込み
	void DoLoad(void)override;

	//初期化
	void DoInit(void)override;

	//更新
	void DoUpdate(void)override;

	//描画(デバッグ)
	void DrawDebug(void);

	//方向転換
	void ChangeDir(void);

	//移動処理
	void Move(void)override;

	//攻撃処理
	void Attack(void)override;
};