#pragma once
#include "DataEditBase.h"
class AddNewData :
    public DataEditBase
{
public:
    AddNewData(void);
    ~AddNewData(void)override;

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    void Reset(void)override;
};

