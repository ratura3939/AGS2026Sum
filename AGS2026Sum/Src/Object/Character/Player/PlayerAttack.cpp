#include "../../../pch.h"
#include"../../Common/Geometry/Sphere.h"
#include "PlayerAttack.h"

PlayerAttack::PlayerAttack(const VECTOR& _playerPos)
	:ActorBase()
	,playerPos_(_playerPos)
{
}

PlayerAttack::~PlayerAttack(void)
{
}

void PlayerAttack::DoInit(void)
{
	
}

void PlayerAttack::DoUpdate(void)
{
}

void PlayerAttack::Draw(void)
{
	//コンボ分岐の表記
}

void PlayerAttack::Release(void)
{
}

void PlayerAttack::HitCollider(std::weak_ptr<Collider> _col)
{
}

void PlayerAttack::Attack(const ATTACK_TYPE& _type)
{
}