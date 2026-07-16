#pragma once
#include<memory>
#include "MakeCutSceneElementBase.h"

class AddNewData;
class EditData;
class MakeFile;

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

private:
    std::unique_ptr<AddNewData>newDataFunc_;
    std::unique_ptr<EditData>editDataFunc_;
    std::unique_ptr<MakeFile>makeFileFunc_;
};

