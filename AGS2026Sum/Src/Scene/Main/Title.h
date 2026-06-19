#pragma once
#include "../SceneBase.h"
#include<memory>

class PixelMaterial;
class PixelRenderer;


class Title : public SceneBase
{
public:
	Title(void);
	~Title(void);

	void Init(void) override;
	void InitUI(void);
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	void ResetUIDirectionParam(void);

#pragma region shader関連
	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>render_;
#pragma endregion

	bool isSelectGameEnd_;	//終了ボタンを選択しているか
};
