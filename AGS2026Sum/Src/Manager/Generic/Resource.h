#pragma once
#include <string>
#include <vector>
#include"../../Lib/nlohmann/json.hpp"

class Resource
{

public:
	// リソースタイプ
	enum class TYPE
	{
		NONE
		,IMG
		,IMGS
		,CSV
		,MODEL
		,SOUND
		,EFFEKSEER
		,JSON
	};

	// コンストラクタ
	Resource(void);
	// コンストラクタ
	Resource(TYPE type, const std::wstring& path);
	// コンストラクタ(IMGS用)
	Resource(TYPE type, const std::wstring& path, int numX, int numY, int sizeX, int sizeY);

	// デストラクタ
	~Resource(void);

	// 読み込み
	void Load(void);

	void LoadCsv(void);

	// 解放
	void Release(void);

	// 複数画像ハンドルを別配列にコピー
	void CopyHandle(int* imgs);

	//JSONデータの取得 : 型を指定して全件取得
	template<typename T>
	T GetData(void) {
		if (resType_ == TYPE::JSON) {
			return jsonData_.get<T>();
		}
		return T();
	}

	// キー指定版 : 名前を指定して1件だけ取得
	template <typename T>
	T GetData(const std::string& key) const
	{
		return jsonData_.at(key).get<T>();
	}

	// リソースタイプ
	TYPE resType_;

	// リソースの読み込み先
	std::wstring path_;

	// 画像とモデルのハンドルID
	int handleId_;

	// IMGS::LoadDivGraph用
	int* handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;

	//Csv用の動的二元
	std::vector<int> dmcHndIdX_;
	std::vector<std::vector<int>> dmcHandleIds_;
	std::vector<std::vector<int>>::iterator csv_;

	// モデル複製用
	std::vector<int> duplicateModelIds_;

private:
	// JSON用
	//GetDataを使用し、最適な方で受け取ってもらうため変数に直接アクセスさせない
	nlohmann::json jsonData_;
};

