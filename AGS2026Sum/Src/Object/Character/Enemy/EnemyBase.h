#pragma once
#include "../CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:

	//コンストラクタ
	EnemyBase(const VECTOR& _initPos, const VECTOR& _movePow);

	//デストラクタ
	~EnemyBase(void);

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

	//衝突後の処理
	void HitCollider(std::weak_ptr<Collider> _col)override;

private:

	//移動量
	const VECTOR initPos_;	
	const VECTOR& movePow_;	

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