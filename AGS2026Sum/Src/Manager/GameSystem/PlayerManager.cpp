#include"../../pch.h"
#include"../Generic/InputManager.h"
#include"../Generic/SceneManager.h"
#include"../Generic/Camera.h"
#include"../Decoration/SoundManager.h"
#include"../GameSystem/AttackManager.h"
#include"../../Scene/Main/Game.h"
#include"../../Utility/Utility.h"
#include "PlayerManager.h"

const std::string PlayerManager::ATTACK_NOMAL = "PlayerAttack";

PlayerManager::PlayerManager(Game& _gameScene, EnemyManager& _enemy, AttackManager& _atk)
	:scene_(_gameScene)
	,atkMng_(_atk)
{
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::Init(void)
{
}

void PlayerManager::Update(AttackManager& _atk)
{
}

void PlayerManager::Draw(void)
{
}

void PlayerManager::Release(void)
{
}