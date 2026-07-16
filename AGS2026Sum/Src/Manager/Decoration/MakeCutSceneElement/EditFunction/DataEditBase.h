#pragma once
class DataEditBase
{
public:
    DataEditBase(void);
    virtual ~DataEditBase(void);

    virtual void Init(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    virtual void Release(void) = 0;

    virtual void Reset(void) = 0;
};

