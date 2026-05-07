#pragma once
#include "../CharacterBase.h"
class PlayerChara :
    public CharacterBase
{
    public:
        PlayerChara(void);
        ~PlayerChara(void);

        void Draw(void)override;			//描画
        void Release(void)override;			//解放

		void HitCollider(std::weak_ptr<Collider> _col)override;	//衝突後の処理
        void InputMoveVec(const VECTOR& _inputVec); //移動入力の受付

    private:
        void DoInit(void)override;			//初期化
        void DoUpdate(void)override;		//更新
		void InitAnim(void)override;		//アニメーションの初期化

		void Move(void)override;			//移動処理
		void Attack(void)override;			//攻撃処理

        VECTOR inputDir_;       //移動入力方向
        float afterMoveRad_;    //最終的なキャラクター角度
		float moveSpeed_;       //移動速度
		bool isMove_;           //移動しているか
		bool isAttack_;         //攻撃しているか  
		std::wstring useAnim_;     //現在のアニメーション
};

