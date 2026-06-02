#pragma once
#include"../../Common/Singleton.h"

class EnemyBase;

class ChunkManager : public Singleton<ChunkManager>
{
	//シングルトン化のため、Singletonクラスをフレンドクラスに指定
	friend class Singleton<ChunkManager>;

public:

	//敵の登録
	void AddEnemy(EnemyBase* _enemy);

	//敵の削除
	void RemoveEnemy(EnemyBase* _enemy);

	//チャンク移動
	void MoveEnemy(EnemyBase* _enemy, const VECTOR& _oldPos);

	//チャンク内の敵の取得
	const std::vector<EnemyBase*>& GetEnemiesInChunk(const VECTOR& _pos) const;

	//指定座標から指定セル分の範囲の敵の取得
	const std::vector<EnemyBase*>& GetEnemiesInChunkWithRange(const VECTOR& _pos, const int _cellRange) const;

private:

	//セルの大きさ
	static constexpr float CELL_SIZE = 100.0f;

	//チャンクの数
	static constexpr int CHUNK_X = 100000 / CELL_SIZE;		//チャンクのXの数
	static constexpr int CHUNK_Z = 100000 / CELL_SIZE;		//チャンクのZの数

	//コンストラクタ
	ChunkManager();

	//デストラクタ
	~ChunkManager()override;

	//チャンクのインデックスを計算
	int GetChunkIndex(const VECTOR& _pos) const;

	//チャンクごとの敵の管理
	std::array<std::vector<EnemyBase*>, CHUNK_X * CHUNK_Z> chunkEnemyMap_;
};

