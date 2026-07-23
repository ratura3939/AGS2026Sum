#pragma once
class AttackCommandInfo
{
public:
	enum class TYPE {
		SEPECIAL	//特殊攻撃
		,ULTIMATE	//必殺技
		,MAX
	};

	//ボタン種類（XBox基準で右から時計回り）
	enum class BUTTON_TYPE {
		B
		,A
		,X
		,Y
		,MAX
	};

	//出現
	void Appear(const TYPE& _type);

	//消す
	void Disappear(void);

	//更新
	void Update(void);

private:
	TYPE useType_;	//使用するタイプ
	int commandImages_[static_cast<int>(TYPE::MAX)];
	int commandGuid_[static_cast<int>(TYPE::MAX)][static_cast<int>(BUTTON_TYPE::MAX)];
};

