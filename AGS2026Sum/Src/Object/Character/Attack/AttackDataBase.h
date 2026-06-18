#pragma once
#include<memory>

class Collider;

struct AttackDataBase
{
	//攻撃属性
	enum class ATTACK_ELEMENT
	{
		NORMAL		//属性なし
		, TACKLE	//突進
		, JUMP		//ジャンプ
	};

	//威力
	float power;

	//攻撃属性
	ATTACK_ELEMENT element;
};