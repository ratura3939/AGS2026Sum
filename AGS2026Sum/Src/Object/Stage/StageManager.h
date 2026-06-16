#pragma once
#include<memory>
#include "../Common/ActorBase.h"

class ModelRenderer;
class ModelMaterial;

class StageManager :
    public ActorBase
{
public:
    StageManager(void);
    ~StageManager(void) override;
    void Draw(void) override;
	void Release(void) override;

	void HitCollider(std::weak_ptr<Collider> _col) override {};

private:
	void DoLoad(void) override;
	void DoInit(void) override;
	void DoUpdate(void) override;

	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;
};

