#pragma once
#include "DataEditBase.h"
class EditData :
    public DataEditBase
{
public:
    EditData(void);
    ~EditData(void)override;

    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    void Release(void)override;

    void Reset(void)override;
};

