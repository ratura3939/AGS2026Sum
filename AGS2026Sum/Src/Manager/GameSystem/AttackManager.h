#pragma once
#include<string>
#include<unordered_set>
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
	//シングルトン化のため、Singletonクラスをフレンドクラスに指定
	friend class Singleton<AttackManager>;

public:

	//攻撃の種類
	enum class ATTACK_TYPE
	{
		P_ATTACK	//プレイヤー攻撃
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
	/// 攻撃コライダの当たったリストを削除する
	/// </summary>
	/// <param name="_col">攻撃コライダ</param>
	void ResetTargetColList(const std::weak_ptr<Collider>& _col);

	/// <summary>
	/// 攻撃が当たるか
	/// </summary>
	/// <param name="_atkCol">当たった攻撃のコライダ</param>
	/// <param name="_hitCol">当たった本体のコライダ</param>
	/// <returns>true:当たる</returns>
	const bool IsCanHit(const std::weak_ptr<Collider>& _atkCol, const std::weak_ptr<Collider>& _hitCol);

	/// <summary>
	/// 攻撃情報を取得
	/// </summary>
	/// <param name="_atkCol">当たった攻撃のコライダ</param>
	/// <param name="_hitCol">当たった本体のコライダ</param>
	/// <returns>攻撃情報</returns>
	const std::weak_ptr<AttackDataBase> GetAttackData(const std::weak_ptr<Collider>& _atkCol, const std::weak_ptr<Collider>& _hitCol);

	/// <summary>
	/// 攻撃データを設定
	/// </summary>
	/// <param name="_name">設定したい攻撃の名前</param>
	/// <param name="_data">設定する攻撃情報</param>
	void SetAttackData(const ATTACK_TYPE& _name, std::shared_ptr<AttackDataBase> _data);

private:

	//攻撃ヒット情報
	struct AttackRuntime
	{
		ATTACK_TYPE name;							//攻撃の種類
		std::unordered_set<Collider*> targetCol;	//あてられた側のコライダ
	};

	//コンストラクタ
	AttackManager(void);

	//デストラクタ
	~AttackManager(void)override;

	//削除
	void Destroy(void)override;

	//攻撃コライダが登録されているか
	const bool IsRegisterCollider(const std::weak_ptr<Collider>& _col);

	std::unordered_map<Collider*, AttackRuntime> colliderAttackTypeList_;							//攻撃判定用コライダーリスト
	std::array<std::shared_ptr<AttackDataBase>, static_cast<int>(ATTACK_TYPE::MAX)> attackDatas_;	//各攻撃の情報リスト
};

