#include"../../pch.h"
#include"../Decoration/SoundManager.h"
#include"../Generic/ResourceManager.h"
#include"../../Object/Character/Attack/AttackDataBase.h"
#include"../../Object/Character/Enemy/Skill/EnemySkillsData.h"
#include "AttackManager.h"

void AttackManager::Load(void)
{
	//ƒŠƒ\[ƒX
	auto& res = ResourceManager::GetInstance();

	//“Gî•ñ
	EnemySkillDatas enemyData = res.Load(ResourceManager::SRC::ENEMY_SKILLS_DATA).GetData<EnemySkillDatas>();

	//Ši”[
	attackDatas_[static_cast<int>(ATTACK_TYPE::E_NORMAL)] = enemyData.skillsData["NormalSkill"];
	attackDatas_[static_cast<int>(ATTACK_TYPE::E_TACKLE)] = enemyData.skillsData["Tackle"];
	attackDatas_[static_cast<int>(ATTACK_TYPE::E_JUMP)] = enemyData.skillsData["Jump"];
}

void AttackManager::AddAttackCollider(const ATTACK_TYPE& _name, const std::weak_ptr<Collider>& _col)
{
	//“o˜^ƒRƒ‰ƒCƒ_‚Ìƒ|ƒCƒ“ƒ^
	auto addCol = _col.lock();
	if (!addCol)return;

	if (colliderAttackTypeList_.contains(addCol.get()))
	{
		//ƒGƒ‰[–h~
		assert(!"‚·‚Å‚É“o˜^‚µ‚Ä‚¢‚é‚à‚Ì‚ğÄ“o˜^‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	//“o˜^
	colliderAttackTypeList_.emplace(addCol.get(), _name);
}

void AttackManager::DeleteAttackCollider(const std::weak_ptr<Collider>& _col)
{
	//íœƒRƒ‰ƒCƒ_
	auto deleteCol = _col.lock();
	if (!deleteCol)return;

	//ˆê’v‚µ‚½‚à‚Ì‚ğÁ‚·
	colliderAttackTypeList_.erase(deleteCol.get());
}

const std::weak_ptr<AttackDataBase> AttackManager::GetAttackData(const std::weak_ptr<Collider>& _col)
{
	//æ“¾‚µ‚½‚¢ƒRƒ‰ƒCƒ_‚Ìƒ|ƒCƒ“ƒ^
	auto getCol = _col.lock();
	if (!getCol)return{};

	//ŠÜ‚Ü‚ê‚Ä‚¢‚é‚©‚ğ’T‚·
	auto find = colliderAttackTypeList_.find(getCol.get());
	if (find == colliderAttackTypeList_.end())
	{
		//Œ©‚Â‚©‚ç‚È‚©‚Á‚½
		assert(!"‘I‘ğ‚³‚ê‚½ƒRƒ‰ƒCƒ_‚Í“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		return {};
	}

	//Œ©‚Â‚©‚Á‚½‚Ì‚ÅUŒ‚î•ñ‚ğ•Ô‚·
	return attackDatas_[static_cast<int>(find->second)];
}

AttackManager::AttackManager(void)
{
}

AttackManager::~AttackManager(void)
{
}

void AttackManager::Destroy(void)
{
	//‘Síœ
	colliderAttackTypeList_.clear();
	attackDatas_.fill(nullptr);
}
