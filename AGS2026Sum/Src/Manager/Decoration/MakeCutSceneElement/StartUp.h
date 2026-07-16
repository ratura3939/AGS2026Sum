#pragma once
#include "MakeCutSceneElementBase.h"
class StartUp :
    public MakeCutSceneElementBase
{
public:
    StartUp(void);
    ~StartUp(void)override;

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    void Reset(void)override;

private:
    bool isStart_;  //開始フラグ

};

