#include"../../pch.h"
#include"../Decoration/SoundManager.h"
#include"../Generic/ResourceManager.h"
#include"../../Object/Character/Attack/AttackDataBase.h"
#include"../../Object/Character/Enemy/Skill/EnemySkillsData.h"
#include "AttackManager.h"

void AttackManager::Load(void)
{
	//リソース
	auto& res = ResourceManager::GetInstance();

	//敵情報
	EnemySkillDatas enemyData = res.Load(ResourceManager::SRC::ENEMY_SKILLS_DATA).GetData<EnemySkillDatas>();

	//格納
	attackDatas_[static_cast<int>(ATTACK_TYPE::E_NORMAL)] = enemyData.skillsData["NormalSkill"];
	attackDatas_[static_cast<int>(ATTACK_TYPE::E_TACKLE)] = enemyData.skillsData["Tackle"];
	attackDatas_[static_cast<int>(ATTACK_TYPE::E_JUMP)] = enemyData.skillsData["Jump"];
}

void AttackManager::AddAttackCollider(const ATTACK_TYPE& _name, const std::weak_ptr<Collider>& _col)
{
	//登録コライダのポインタ
	auto addCol = _col.lock();
	if (!addCol)return;

	if (IsRegisterCollider(_col))
	{
		//エラー防止
		assert(!"すでに登録しているものを再登録しようとしています");
		return;
	}

	//登録
	colliderAttackTypeList_[addCol.get()].name = _name;
}

void AttackManager::DeleteAttackCollider(const std::weak_ptr<Collider>& _col)
{
	//削除コライダ
	auto deleteCol = _col.lock();
	if (!deleteCol)return;

	//一致したものを消す
	colliderAttackTypeList_.erase(deleteCol.get());
}

void AttackManager::ResetTargetColList(const std::weak_ptr<Collider>& _col)
{
	//攻撃コライダ
	auto col = _col.lock();
	if (!col)return;

	//含まれているかを探す
	auto colPtr = col.get();
	if (!IsRegisterCollider(_col))
	{
		//見つからなかった
		return;
	}

	//リセット
	colliderAttackTypeList_[colPtr].targetCol.clear();
}

const bool AttackManager::IsCanHit(const std::weak_ptr<Collider>& _atkCol, const std::weak_ptr<Collider>& _hitCol)
{
	//コライダのポインタ
	auto atkCol = _atkCol.lock();
	auto hitCol = _hitCol.lock();
	if (!atkCol || !hitCol)return false;

	//含まれているかを探す
	auto atkColPtr = atkCol.get();
	if (!IsRegisterCollider(_atkCol))
	{
		//見つからなかった
		assert(!"選択されたコライダは登録されていません");
		return false;
	}

	//単体ヒット　かつ　既に攻撃済みリストに当たったコライダが登録されているかを調べる
	auto hitColPtr = hitCol.get();
	bool isMultiHit = attackDatas_[static_cast<int>(colliderAttackTypeList_[atkColPtr].name)]->isMultiHit;
	bool isRegist = colliderAttackTypeList_[atkColPtr].targetCol.contains(hitColPtr);
	if (!isMultiHit && isRegist)
	{
		//当たらない
		return false;
	}

	//当たる
	return true;
}

const std::weak_ptr<AttackDataBase> AttackManager::GetAttackData(const std::weak_ptr<Collider>& _atkCol, const std::weak_ptr<Collider>& _hitCol)
{
	//コライダのポインタ
	auto atkCol = _atkCol.lock();
	auto hitCol = _hitCol.lock();
	if (!atkCol || !hitCol)return {};

	//ポインタ変換
	auto atkColPtr = atkCol.get();

	//含まれているか
	if(!IsRegisterCollider(_atkCol))
	{ 
		assert(!"選択されたコライダは登録されていません");
		return {};
	}

	//見つかったので攻撃済みリストに当たった側を保存する
	auto hitColPtr = hitCol.get();
	colliderAttackTypeList_[atkColPtr].targetCol.insert(hitColPtr);

	//攻撃情報を返す
	return attackDatas_[static_cast<int>(colliderAttackTypeList_[atkColPtr].name)];
}

void AttackManager::SetAttackData(const ATTACK_TYPE& _name, std::shared_ptr<AttackDataBase> _data)
{
	//攻撃情報を上書き
	attackDatas_[static_cast<int>(_name)] = _data;
}

AttackManager::AttackManager(void)
{
}

AttackManager::~AttackManager(void)
{
}

void AttackManager::Destroy(void)
{
	//全削除
	colliderAttackTypeList_.clear();
	attackDatas_.fill(nullptr);
}

const bool AttackManager::IsRegisterCollider(const std::weak_ptr<Collider>& _col)
{
	//含まれているかを探す
	auto colPtr = _col.lock().get();
	if (!colliderAttackTypeList_.contains(colPtr))
	{
		//見つからなかった
		return false;
	}

	//見つかった
	return true;
}