#pragma once
#include "MakeCutSceneElementBase.h"
class DataEditBase :
    public MakeCutSceneElementBase
{
public:
    DataEditBase(void);
    virtual ~DataEditBase(void)override;

    virtual void Init(void)override = 0;
    virtual void Update(void)override = 0;
    virtual void Draw(void)override = 0;
    virtual void Release(void)override = 0;

    virtual void Reset(void)override = 0;
};

