#pragma once
#include<memory>
#include "../Common/ActorBase.h"

class ModelRenderer;
class ModelMaterial;

class StageObjectBase : public ActorBase
{
public:

	//コンストラクタ
	StageObjectBase(int _modelId,const VECTOR& _position);

	//デストラクタ
	~StageObjectBase(void)override;

	//描画
	virtual void Draw(void)override;
	
	//解放
	virtual void Release(void)override;

	//衝突後の処理
	virtual void HitCollider(std::weak_ptr<Collider> _col)override;

protected:

	//読み込み
	virtual void DoLoad(void)override;		

	//初期化
	virtual void DoInit(void)override;		

	//更新
	virtual void DoUpdate(void)override;	

	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;
};

