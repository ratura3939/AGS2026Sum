#pragma once
#include<string>
#include<memory>
#include<DxLib.h>
#include"../../../Common/Quaternion.h"
#include"PlayerAttack.h"
#include"../CharacterBase.h"

class Game;
class EnemyManager;
class PlayerChara;

class PlayerManager
{
public:
#pragma region アニメーション登録名
	static const std::wstring ANIM_IDLE;		//待機
	static const std::wstring ANIM_RUN;			//移動
	static const std::wstring ANIM_DAMAGE;		//ダメージ
	static const std::wstring ANIM_FIRST_PUNCH;	//初回パンチ
	static const std::wstring ANIM_SECOND_PUNCH;//二回目パンチ
	static const std::wstring ANIM_THIRD_PUNCH;	//三回目パンチ
	static const std::wstring ANIM_MIDDLE_KICK;	//中段キック(二段目を強派生)
	static const std::wstring ANIM_HIGH_KICK;	//上段キック(三段目を強派生)
	static const std::wstring ANIM_FINSH_KICK;	//最終段キック（パンチ三回目後、強派生）
#pragma endregion

	PlayerManager(Game& _gameScene);
	~PlayerManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//位置・回転取得
	const VECTOR& GetPos(void)const;		//座標
	const VECTOR& GetFocusPos(void);		//注視点
	const Quaternion& GetQua(void);			//回転
	//const VECTOR& GetFocusPoint(void);	//注視点

	//const bool IsAlive(void)const;

	//現在の攻撃の吹っ飛び方取得
	const CharacterBase::KNOCKBACK_TYPE GetCurrentKnockBackType(void)const;

	//アニメーションの更新速度設定
	void SetAnimSpeedPercent(const float _percent);

	//特殊攻撃準備フラグ設定
	void SetIsSpecialRedy(const bool _flag) { isSpecialAttackRedy_ = _flag; }

private:
	void UserInput(void);					//入力受付
	void SetAttackStateForCharacter(void);	//攻撃に関する状態をキャラクターに反映
	const bool Attack(PlayerAttack::ATTACK_TYPE _type);	//攻撃判定の設定処理(返り値　true=成功/false=失敗)
	const bool AttackSpecial(PlayerAttack::ATTACK_TYPE _type);	//攻撃判定の設定処理(返り値　true=成功/false=失敗)

	Game& scene_;	//ゲームクラス参照
	VECTOR focusPos_;	//注視点

	std::shared_ptr<PlayerChara> character_;	//キャラクター
	std::unique_ptr<PlayerAttack> attack_;		//攻撃
	bool isRefuseAttackInput_;	//攻撃入力を受け付けない状態か
	bool isForcePlayAnim_;		//強制再生させるか(攻撃の初段のみ強制再生)

	bool isSpecialAttackRedy_;	//特殊攻撃の準備ができているか
};

