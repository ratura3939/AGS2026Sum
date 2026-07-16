#pragma once
#include "MakeCutSceneElementBase.h"

class EditDataScene :
    public MakeCutSceneElementBase
{
public:
    EditDataScene(void);
    ~EditDataScene(void)override;

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    void Reset(void)override;
};

