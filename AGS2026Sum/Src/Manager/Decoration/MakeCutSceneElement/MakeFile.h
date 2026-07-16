#pragma once
#include "MakeCutSceneElementBase.h"
class MakeFile :
    public MakeCutSceneElementBase
{
public:
    MakeFile(void);
    ~MakeFile(void)override;

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    void Reset(void)override;
};

