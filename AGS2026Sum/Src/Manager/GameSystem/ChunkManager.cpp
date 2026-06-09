#include "../../pch.h"
#include "../../Object/Character/Enemy/EnemyBase.h"
#include "ChunkManager.h"

ChunkManager::ChunkManager()
{
}

ChunkManager::~ChunkManager()
{
	chunkEnemyMap_.fill({});
}

int ChunkManager::GetChunkIndex(const VECTOR& _pos) const
{
	//座標からセルの座標を求める
	int cellX = static_cast<int>(std::floor(_pos.x / CELL_SIZE));
	int cellZ = static_cast<int>(std::floor(_pos.z / CELL_SIZE));
	int index = cellX + cellZ * CHUNK_X;

	//セルがチャンクの範囲外ならエラーを返す
	if (cellX < 0 || cellX >= CHUNK_X || cellZ < 0 || cellZ >= CHUNK_Z)
	{
		assert(!"Out of Chunk");
		return -1;	//エラー
	}

	return index;
}

void ChunkManager::DebugDraw(void) const
{
	//チャンクの線を描画
	for (int z = 0; z <= CHUNK_Z; z++)
	{
		DrawLine3D(
			VGet(0.0f, 0.0f, z * CELL_SIZE),
			VGet(CHUNK_X * CELL_SIZE, 0.0f, z * CELL_SIZE),
			GetColor(255, 255, 255));
	}

	for (int x = 0; x <= CHUNK_X; x++)
	{
		DrawLine3D(
			VGet(x * CELL_SIZE, 0.0f, 0.0f),
			VGet(x * CELL_SIZE, 0.0f, CHUNK_Z * CELL_SIZE),
			GetColor(255, 255, 255));
	}

}

void ChunkManager::AddEnemyGroup(EnemyGroup* _enemyGroup)
{
	//敵の座標からセルの座標を求める
	int index = GetChunkIndex(_enemyGroup->GetPos());

	//チャンクに登録
	chunkEnemyMap_[index].push_back(_enemyGroup);

	printfDx(
		L"Pos(%.0f %.0f) Index=%d\n",
		_enemyGroup->GetPos().x,
		_enemyGroup->GetPos().z,
		index);

	//敵グループにチャンクの添え字を設定
	_enemyGroup->SetChunkIndex(index);
}

void ChunkManager::RemoveEnemyGroup(EnemyGroup* _enemyGroup)
{
	//敵の座標からセルの座標を求める
	int index = _enemyGroup->GetChunkIndex();

	//チャンクから削除
	auto& group = chunkEnemyMap_[index];
	group.erase(std::remove(group.begin(), group.end(), _enemyGroup), group.end());
}

void ChunkManager::MoveEnemyGroup(EnemyGroup* _enemyGroup, const VECTOR& _oldPos)
{
	//古い座標からセルの座標を求める
	int oldIndex = _enemyGroup->GetChunkIndex();

	//新しい座標からセルの座標を求める
	int newIndex = GetChunkIndex(_enemyGroup->GetPos());

	//セルが変わっていない場合は何もしない
	if (oldIndex == newIndex) return;

	//古いチャンクから削除
	auto& oldGroup = chunkEnemyMap_[oldIndex];
	oldGroup.erase(std::remove(oldGroup.begin(), oldGroup.end(), _enemyGroup), oldGroup.end());

	//新しいチャンクに登録
	chunkEnemyMap_[newIndex].push_back(_enemyGroup);

	//敵グループにチャンクの添え字を再設定
	_enemyGroup->SetChunkIndex(newIndex);
}

const std::vector<EnemyGroup*>& ChunkManager::GetEnemyGroupInChunk(const VECTOR& _pos) const
{
	//座標からセルの座標を求める
	int index = GetChunkIndex(_pos);

	//チャンク内の敵を返す
	return chunkEnemyMap_[index];
}

void ChunkManager::GetEnemyGroupsInRangeChunk(std::vector<EnemyGroup*>& _enemyGroups, const VECTOR& _pos, const int _cellRange) const
{
	//座標からセルの座標を求める
	int index = GetChunkIndex(_pos);
	int centerX = index % CHUNK_X;
	int centerZ = index / CHUNK_X;

	//セル範囲分ループ
	for (int i = -_cellRange; i <= _cellRange; i++)
	{
		for(int j = -_cellRange; j <= _cellRange; j++)
		{
			//セルの座標を求める
			int cellX = centerX + i;
			int cellZ = centerZ + j;

			//セルがチャンクの範囲外ならスキップ
			if(cellX < 0 || cellX >= CHUNK_X || cellZ < 0 || cellZ >= CHUNK_Z) continue;
			
			//セルの添え字を求める
			int chunkIndex = cellX + cellZ * CHUNK_X;
			const auto& enemies = chunkEnemyMap_[chunkIndex];

			//セル内の敵を結果に追加
			_enemyGroups.insert(_enemyGroups.end(), enemies.begin(), enemies.end());
		}
	}
}
