#pragma once
#include<DxLib.h>
#include<set>
#include<memory>
#include<string>
#include"../Common/Geometry/Geometry.h"

class ActorBase;

class Collider
{
public:

	//タグ
	enum class COL_TAG
	{
		OBJECT
		,ENEMY
		,MAX
	};

	Collider(ActorBase& _master, const COL_TAG _tags, std::unique_ptr<Geometry> _geo, const std::set<COL_TAG> _hitTags = {});
	~Collider(void);

	//衝突処理
	void OnHit(std::weak_ptr<Collider> _col);

	//タイプの取得
	const COL_TAG& GetTags(void)const { return tag_; }
	const std::set<COL_TAG>& GetHitTags(void)const { return hitTags_; }

	//タグの追加
	void AddHitTags(const COL_TAG& _tag) { hitTags_.insert(_tag); }

	//タグの削除
	void DeleteHitTags(const COL_TAG& _tag);

	//タグの検索
	const bool IsContainsTag(const COL_TAG& _tag)const { return tag_ == _tag; }
	const bool IsContainsHitTags(const COL_TAG& _tag)const { return hitTags_.contains(_tag); }
	const bool IsContainsHitTags(const std::set<COL_TAG>& _tags);

	//形状の取得
	Geometry& GetGeometry(void) const { return *geometry_; }

	//衝突判定に使用するか
	const bool IsUseThis(void) const { return isCollision_; }

	//衝突判定の使用設定
	void SetUseThis(const bool _isUse) { isCollision_ = _isUse; }

	//持ち主の描画設定
	void SetMasterIsDraw(const bool _isDraw);

	//デバッグ
	void DrawDebugCollider(void);

	//判定の削除(必ずこれで削除すること)
	void Kill(void) { isDead_ = true; }

	//削除するか
	const bool IsDead(void) const { return isDead_; }

	//当たっているか
	const bool IsHit(void)const { return isHit_; }

protected:

	//持ち主
	ActorBase& master_;

	//形状
	std::unique_ptr<Geometry> geometry_;

	//自身のタグ
	COL_TAG tag_;

	//衝突判定を行うタグ
	std::set<COL_TAG> hitTags_;

	//衝突判定を行うか
	bool isCollision_;

	//衝突の有無
	bool isHit_;

	//生存判定
	bool isDead_;
};

