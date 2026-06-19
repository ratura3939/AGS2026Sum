#pragma once
#include<vector>
#include<memory>
#include"../../Common/Singleton.h"
#include"AttackManager.h"
#include"../../Object/Common/Collider.h"

//オブジェクトのコライダーを各自登録するために、シングルトン化
class CollisionManager : public Singleton<CollisionManager>
{
	//シングルトン化のため、Singletonクラスをフレンドクラスに指定
	friend class Singleton<CollisionManager>;

public:

	//要素追加
	void AddCollider(std::weak_ptr<Collider> _col);
	
	//コライダの全削除
	void DeleteAllCollider(void);

	//すべてのコライダーの衝突判定
	void UpdateColliders(void);

private:

	CollisionManager(void);
	~CollisionManager(void)override;
	
	//要素削除
	void DeleteCollider(void);

	//コライダーの形状衝突判定
	void CollisionGeometry(std::weak_ptr<Collider> _col1, std::weak_ptr<Collider> _col2);

	//タグごとの判定
	const bool CheckCollisionTags(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2);

	/// <summary>
	/// 判定前の処理
	/// </summary>
	/// <param name="_col1">コライダ１</param>
	/// <param name="_col2">コライダ２</param>
	/// <returns>true:当たり判定をする</returns>
	const bool PreCollision(std::weak_ptr<Collider> _col1, std::weak_ptr<Collider> _col2);

	bool isSlow_;	//スロー演出するか

	std::vector<std::weak_ptr<Collider>> colliders_;
};

