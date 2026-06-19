#pragma once
#include "../SceneBase.h"
#include<memory>

class PixelMaterial;
class PixelRenderer;

class GameOver :	public SceneBase
{
public:

	GameOver(void);
	~GameOver(void)override;

	void Init(void) override;
	void InitUI(void);
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Reset(void)override;

private:
	void InitSound(void)override;
	void InitEffect(void)override;

	std::unique_ptr<PixelMaterial>material_;
	std::unique_ptr<PixelRenderer>render_;
};

