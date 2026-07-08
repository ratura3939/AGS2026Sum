#pragma once
#include "SingletonRegistry.h"

template<typename T>
class Singleton
{
public:

	//コピー禁止
	Singleton(const Singleton& _copy) = delete;
	Singleton& operator=(const Singleton& _copy) = delete;

	/// <summary>
	/// 明示的にインスタンスの生成
	/// </summary>
	/// <param name="_timing">解放タイミング</param>
	static inline void CreateInstance(SingletonRegistry::DESTROY_TIMING _timing)
	{
		if (instance_ != nullptr)return;
		
		//生成
		instance_ = new T();

		//読み込みと初期化
		instance_->Load();
		instance_->Init();

		//レジスタに格納
		SingletonRegistry::GetInstance().RegistryDestroyer(_timing, [](void) { instance_->DestroyInstance(); });
	}

	//インスタンスの取得
	static inline T& GetInstance(void)
	{
		if (instance_ == nullptr)
		{
			CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);
		}

		return *instance_;
	}

	//読み込み
	virtual void Load(void) {}

	//初期化
	virtual void Init(void) {}

	//インスタンスの破棄
	inline void DestroyInstance(void)
	{
		if (instance_ == nullptr)return;

		//破棄時の処理
		instance_->Destroy();

		//破棄
		delete instance_;
		instance_ = nullptr;
	}

protected:

	//静的インスタンス
	static T* instance_;

	//コンストラクタ
	Singleton(void) = default;

	//デストラクタ
	virtual ~Singleton(void) = default;

	//削除時処理
	virtual void Destroy(void){}
};

//静的インスタンスの初期化
template<typename T>
T* Singleton<T>::instance_ = nullptr;