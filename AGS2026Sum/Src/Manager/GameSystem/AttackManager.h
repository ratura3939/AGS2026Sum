#pragma once
#include<string>
#include<memory>
#include<vector>
#include<array>
#include<DxLib.h>
#include"../../Common/Singleton.h"
#include"../../Common/Quaternion.h"
#include"../../Object/Common/Collider.h"
#include"../../Object/Character/Attack/AttackDataBase.h"
#include"../Decoration/SoundManager.h"

class AttackManager : public Singleton<AttackManager>
{
	//シングルトン化のため共有化
	friend class Singleton<AttackManager>;

public:

	//攻撃の種類
	enum class ATTACK_TYPE
	{
		P_PUNCH		//プレイヤーパンチ
		, E_NORMAL	//敵通常
		, E_TACKLE	//敵突進
		, E_JUMP	//敵ジャンプ
		, MAX
	};

	//外部読み込み
	void Load(void)override;

	/// <summary>
	/// 攻撃用コライダーの登録
	/// </summary>
	/// <param name="_name">攻撃データ名</param>
	/// <param name="_col">攻撃用コライダ</param>
	void AddAttackCollider(const ATTACK_TYPE& _name, const std::weak_ptr<Collider>& _col);

	/// <summary>
	/// コライダリストから削除
	/// </summary>
	/// <param name="_col">削除するコライダ</param>
	void DeleteAttackCollider(const std::weak_ptr<Collider>& _col);

	/// <summary>
	/// 攻撃情報を取得
	/// </summary>
	/// <param name="_col">情報を取得してきたいコライダ</param>
	/// <returns>攻撃情報</returns>
	const std::weak_ptr<AttackDataBase> GetAttackData(const std::weak_ptr<Collider>& _col);

private:

	//コンストラクタ
	AttackManager(void);

	//デストラクタ
	~AttackManager(void)override;

	//削除
	void Destroy(void)override;

	std::unordered_map<Collider*, ATTACK_TYPE> colliderAttackTypeList_;								//攻撃判定用コライダーリスト
	std::array<std::shared_ptr<AttackDataBase>, static_cast<int>(ATTACK_TYPE::MAX)> attackDatas_;	//各攻撃の情報リスト
};

