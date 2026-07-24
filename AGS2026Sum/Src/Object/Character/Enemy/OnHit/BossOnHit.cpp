#include "../../../../pch.h"
#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/GameSystem/AttackManager.h"
#include"../../../../Manager/GameSystem/ComboManager.h"
#include"../../../../Manager/Decoration/EffectManager.h"
#include"../../../../Manager/Decoration/SoundManager.h"
#include "../../Player/ToJson/PlayerAttackData.h"
#include "../State/EnemyStaggerState.h"
#include "../Skill/EnemySkillBase.h"
#include "../EnemyBase.h"
#include "../Types/Boss/MiddleBoss.h"
#include "../Types/Boss/BossBattleComponent.h"
#include "BossOnHit.h"

BossOnHit::BossOnHit(EnemyBase& _parent)
	: EnemyOnHit(_parent)
{
}

BossOnHit::~BossOnHit(void)
{
}

void BossOnHit::CalcDamage(const std::weak_ptr<Collider> _col)
{
	//親をボスに
	auto parent = dynamic_cast<BossBase*>(&parent_);
	if (!parent)return;

	//終了なら無視
	if (parent_.IsFade())return;

	//攻撃マネージャー
	auto& atkMng = AttackManager::GetInstance();

	//ヒット者
	const auto& col = _col.lock();

	//自身のコライダ
	const auto& myCol = parent_.GetColliders()[0];

	//本体にあたってないので無視
	if (!myCol->IsHit())return;

	//当たれるか
	if (!atkMng.IsCanHit(col, myCol)) return;

	//攻撃情報
	const auto& atkData = atkMng.GetAttackData(_col, myCol).lock();
	auto data = dynamic_pointer_cast<AttackData>(atkData);

	//攻撃間隔
	if (data->hitInterval > cnt_ && data->isMultiHit)
	{
		//カウンタ
		auto& scnMng = SceneManager::GetInstance();
		cnt_ += scnMng.GetScaleUpdateSpeedRate(scnMng.GetDeltaTime());
		return;
	}

	//ここからヒット処理

	//ヒットエフェクト
	auto& eff = EffectManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	eff.Play(parent_.GetSpeciesName(), EffectManager::EFFECT_NAME::ENEMY_HIT, parent_.GetPos(), parent_.GetQua(), 10.0f);

	//リセット
	cnt_ = 0.0f;

	//回転情報
	const Quaternion quaRot = col->GetGeometry().GetColRot();

	//吹っ飛び(相手の向いている方向)
	VECTOR blowPow = quaRot.GetForward();

	//ガードブレイク値
	float breakValue = data->guardBreakPower;

	//戦闘情報
	BossBattleComponent& battle = parent->GetBattleComponent();

	bool isAttackCancel = false;

	//敵の現在攻撃
	const auto enemySkill = parent_.GetCurrentSkill();
	if (enemySkill != nullptr)
	{
		//属性が一致したなら削り値上昇と攻撃キャンセル
		const auto skillElement = enemySkill->GetAttackElement();
		if (skillElement == data->element && skillElement != AttackDataBase::ATTACK_ELEMENT::NORMAL)
		{
			breakValue *= 10.0f;
			isAttackCancel = true;

			//SE
			snd.Play(SoundManager::SOUND_NAME::ENEMY_SKILL_CANCEL_SE);
		}
	}

	//ガードブレイク
	battle.GuardBreak(breakValue);

	//スタン判定
	bool isGuardBreak = battle.IsGuardBreak();
	
	//ダメージ状態
	std::string knockback = data->knockBackType;

	//ダメージ威力
	float power = data->power;

	//スタンしたなら攻撃キャンセル
	if (isGuardBreak && !battle.IsBreakEnd())isAttackCancel = true;

	if (isAttackCancel)
	{
		//通常状態
		parent_.ChangeAction(ENEMY_ACTION::ATTACK_END);

		//ひるむ
		parent_.PlayAnim(L"Stagger");
	}

	//スタンしていないならダメージを減らす
	if(!isGuardBreak)power *= 0.1f;

	//ダメージ処理
	parent_.Damage(power);

	//スタン中は状態遷移をしない
	if (isGuardBreak || !parent_.IsAlive())
	{
		//各吹っ飛び
		parent_.ChangeState((this->*createState_[knockback])(blowPow));
	}

	//コンボカウント
	ComboManager::GetInstance().AddCombo();
}