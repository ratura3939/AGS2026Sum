#include"../../pch.h"
#include"../../Application.h"
#include"../Generic/ResourceManager.h"
#include"../GameSystem/AttackManager.h"
#include"../../Scene/Main/Game.h"
#include"../../Utility/Utility.h"
#include "EnemyManager.h"

const std::string EnemyManager::ATTACK_NOMAL = "EnemyAttack";

EnemyManager::EnemyManager(Game& _scene, AttackManager& _atk)
	:gameScene_(_scene)
	,atkMng_(_atk)
{
	enemyCnt_ = -1;
	numImg_ = nullptr;
	platePos_ = Utility::VECTOR_INIT;
	preBattle_ = false;
	createBoss_ = false;
}

EnemyManager::~EnemyManager(void)
{
}

void EnemyManager::Init(const VECTOR& _pPos)
{
}

void EnemyManager::Update(const VECTOR& _playerPos, AttackManager& _atkMng)
{
}

void EnemyManager::UpdateAnim(void)
{
}

void EnemyManager::Draw(void)
{
}

void EnemyManager::Release(void)
{
}