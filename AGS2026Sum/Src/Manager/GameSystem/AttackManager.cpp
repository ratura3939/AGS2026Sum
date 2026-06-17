#include"../../pch.h"
#include"../Decoration/SoundManager.h"
#include"../../Object/Character/Attack/AttackDataBase.h"
#include "AttackManager.h"

void AttackManager::AddAttackCollider(const std::wstring& _name, const std::weak_ptr<AttackDataBase> _data)
{
	auto& adress = *this;

	//Šù‚É—v‘f‚ª‚ ‚é‚Æ‚«
	if(attackColliders_.contains(_name)){
		//ƒGƒ‰[–h~
		assert("‚·‚Å‚É“o˜^‚µ‚Ä‚¢‚é‚à‚Ì‚ğÄ“o˜^‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚Ü‚·");
		return;
	}

	attackColliders_.emplace(_name, _data);
}

void AttackManager::DeleteAttackCollider(const std::wstring& _name)
{
	attackColliders_.erase(_name);
}

void AttackManager::DeleteCollider(const std::wstring& _name)
{
	attackColliders_.erase(_name);
}

void AttackManager::UseAttackCollision(const std::wstring& _name)
{
	attackColliders_.at(_name).lock()->col.lock()->SetUseThis(false);
}

const std::weak_ptr<AttackDataBase> AttackManager::GetAttackData(const std::wstring& _name)
{
	//—v‘f‚ª‚ ‚é‚©‚Ç‚¤‚©
	if (!attackColliders_.contains(_name))
	{
		//‚È‚¢‚Ì‚Å‹ó
		assert(L"not exist attack");
		return std::weak_ptr<AttackDataBase>();
	}

	//UŒ‚î•ñ‚ğ•Ô‚·
	return attackColliders_[_name];
}
