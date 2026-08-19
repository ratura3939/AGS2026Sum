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

	AttackCommandInfo(void);
	~AttackCommandInfo(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//出現
	void Appear(const TYPE& _type);

	//消去
	void Disappear(const TYPE& _type);

private:
	void UpdateNone(void);		//空の更新
	void UpdateAppear(void);	//出現更新
	void UpdateShowGuide(void);	//効果見せ
	void UpdateDisappear(void);	//消失更新

	void DrawCommand(void);		//４ボタンの表示
	void DrawGuide(void);		//効果の表示

	//消失
	void DoDisappear(void);

	using Update_f = void(AttackCommandInfo::*)(void);
	//更新処理
	Update_f useUpdate_;

	TYPE useType_;	//使用するタイプ
	int commandImages_[static_cast<int>(TYPE::MAX)];	//４ボタン画像
	int commandGuides_[static_cast<int>(TYPE::MAX)][static_cast<int>(BUTTON_TYPE::MAX)];	//それぞれの効果

	bool isAppeared_;	//出現しているか
	bool isDrawGuide_;	//ガイドを表示するか
	double guideIconExRate_;	//ガイド項目の描画倍率

	VECTOR centerPos_;	//位置
	VECTOR goalPos_;	//目標位置
};

