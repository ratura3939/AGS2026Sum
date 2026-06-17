#pragma once
#include<string>
#include<memory>
#include<vector>
#include<unordered_map>
#include<map>
#include<DxLib.h>
#include"../../Common/Singleton.h"
#include"../../Common/Quaternion.h"
#include"../../Object/Common/Collider.h"
#include"../../Object/Character/Attack/AttackDataBase.h"
#include"../Decoration/SoundManager.h"

class AttackManager : public Singleton<AttackManager>
{
	//ƒVƒ“ƒOƒ‹ƒgƒ“‰»‚Ì‚½‚ß‹¤—L‰»
	friend class Singleton<AttackManager>;

public:

	/// <summary>
	/// UŒ‚—pƒRƒ‰ƒCƒ_[‚Ì“o˜^
	/// </summary>
	/// <param name="_name">“o˜^–¼</param>
	/// <param name="_data">UŒ‚ƒf[ƒ^</param>
	void AddAttackCollider(const std::wstring& _name, const std::weak_ptr<AttackDataBase> _data);

	/// <summary>
	/// UŒ‚î•ñíœ
	/// </summary>
	/// <param name="_name"></param>
	void DeleteAttackCollider(const std::wstring& _name);
	void DeleteCollider(const std::wstring& _name);

	//UŒ‚‚Ì”»’è‚ªg—p‚³‚ê‚½‚Æ‚«
	void UseAttackCollision(const std::wstring& _name);

	/// <summary>
	/// UŒ‚î•ñ‚ğæ“¾
	/// </summary>
	/// <param name="_name">“o˜^–¼</param>
	/// <returns>UŒ‚î•ñ</returns>
	const std::weak_ptr<AttackDataBase> GetAttackData(const std::wstring& _name);

private:

	std::unordered_map<std::wstring, std::weak_ptr<AttackDataBase>> attackColliders_; //UŒ‚”»’è—pƒRƒ‰ƒCƒ_[
};

