#pragma once
#include "MakeCutSceneElementBase.h"
class SelectFunction :
    public MakeCutSceneElementBase
{
public:
    SelectFunction(void);
    ~SelectFunction(void)override;

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    void Reset(void)override;
};

