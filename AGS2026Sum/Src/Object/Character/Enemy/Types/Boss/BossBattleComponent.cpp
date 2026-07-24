#include"../../../../../pch.h"
#include"../../../../../Manager/Generic/SceneManager.h"
#include"../../../../../Manager/Decoration/SoundManager.h"
#include"BossBattleComponent.h"

BossBattleComponent::BossBattleComponent(void)
{
	Reset();
}

BossBattleComponent::~BossBattleComponent(void)
{
}

void BossBattleComponent::Update(void)
{
	//シーンマネージャー
	auto& scnMng = SceneManager::GetInstance();

	//ガードブレイクされた
	if (IsGuardBreak())
	{
		//ブレイク時間の更新
		breakTime_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());

		if (breakTime_ >= BREAK_TIME)
		{
			Reset();
		}
	}
}

void BossBattleComponent::Draw(void)
{
	//ガード耐久
	//DrawFormatString(0, 0, 0xff0000, L"Guard Durability: %.2f", guardDurability_);
}

void BossBattleComponent::Reset(void)
{
	//初期化
	guardDurability_ = GUARD_DURABILITY_MAX;
	breakTime_ = 0.0f;
}

void BossBattleComponent::GuardBreak(const float _value)
{
	//ブレイク済みなら何もしない
	if (IsGuardBreak())return;

	guardDurability_ -= _value;

	//ブレイクになった瞬間
	if (IsGuardBreak())SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::ENEMY_GUARD_BREAK_SE);
}