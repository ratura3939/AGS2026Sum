#include "../../pch.h"
#include "../../Object/Character/Enemy/EnemyBase.h"
#include "ChunkManager.h"

ChunkManager::ChunkManager()
{
}

ChunkManager::~ChunkManager()
{
}

int ChunkManager::GetChunkIndex(const VECTOR& _pos) const
{
	//座標からセルの座標を求める
	int cellX = static_cast<int>(_pos.x / CELL_SIZE);
	int cellZ = static_cast<int>(_pos.z / CELL_SIZE);
	int index = cellX + cellZ * CHUNK_X;

	return 0;
}

void ChunkManager::AddEnemy(EnemyBase* _enemy)
{
	//敵の座標からセルの座標を求める
	int index = GetChunkIndex(_enemy->GetPos());

	//チャンクに登録
	chunkEnemyMap_[index].push_back(_enemy);
}

void ChunkManager::RemoveEnemy(EnemyBase* _enemy)
{
	//敵の座標からセルの座標を求める
	int index = GetChunkIndex(_enemy->GetPos());

	//チャンクから削除
	auto& enemies = chunkEnemyMap_[index];
	enemies.erase(std::remove(enemies.begin(), enemies.end(), _enemy), enemies.end());
}

void ChunkManager::MoveEnemy(EnemyBase* _enemy, const VECTOR& _oldPos)
{
	//古い座標からセルの座標を求める
	int oldIndex = GetChunkIndex(_oldPos);

	//新しい座標からセルの座標を求める
	int newIndex = GetChunkIndex(_enemy->GetPos());

	//セルが変わっていない場合は何もしない
	if (oldIndex == newIndex) return;

	//古いチャンクから削除
	auto& oldEnemies = chunkEnemyMap_[oldIndex];
	oldEnemies.erase(std::remove(oldEnemies.begin(), oldEnemies.end(), _enemy), oldEnemies.end());

	//新しいチャンクに登録
	chunkEnemyMap_[newIndex].push_back(_enemy);
}

const std::vector<EnemyBase*>& ChunkManager::GetEnemiesInChunk(const VECTOR& _pos) const
{
	//座標からセルの座標を求める
	int index = GetChunkIndex(_pos);

	//チャンク内の敵を返す
	return chunkEnemyMap_[index];
}

const std::vector<EnemyBase*>& ChunkManager::GetEnemiesInChunkWithRange(const VECTOR& _pos, const int _cellRange) const
{
	std::vector<EnemyBase*> ret;

	//座標からセルの座標を求める
	int index = GetChunkIndex(_pos);

	//セル範囲分ループ
	for (int i = -_cellRange; i <= _cellRange; i++)
	{
		for(int j = -_cellRange; j <= _cellRange; j++)
		{
			//セルの座標を求める
			int cellX = index + i;
			int cellZ = index + j;

			//セルがチャンクの範囲外ならスキップ
			if(cellX < 0 || cellX >= CHUNK_X || cellZ < 0 || cellZ >= CHUNK_Z) continue;
			
			//セルの添え字を求める
			int chunkIndex = cellX + cellZ * CHUNK_X;
			const auto& enemies = chunkEnemyMap_[chunkIndex];

			//セル内の敵を結果に追加
			ret.insert(ret.end(), enemies.begin(), enemies.end());
		}
	}

	//範囲内の敵を返す
	return ret;
}
