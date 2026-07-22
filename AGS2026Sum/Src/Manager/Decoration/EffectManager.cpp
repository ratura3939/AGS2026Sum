#include"../../pch.h"
#include "EffectManager.h"

EffectManager* EffectManager::instance_ = nullptr;

void EffectManager::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new EffectManager();
	}
}

EffectManager& EffectManager::GetInstance(void)
{
	return *instance_;
}

EffectManager::EffectManager(void) {

}

void EffectManager::Add(const EFFECT_NAME& _name, int _data)
{
	//連想配列内にすでに要素が入っているかを検索
	//入っていたら処理終了
	if (effectRes_.find(_name) != effectRes_.end())return;

	//新規データのため情報を追加
	effectRes_.emplace(_name, _data);
}

int EffectManager::Play(const std::wstring& _master,const EFFECT_NAME& _name, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed, const SoundManager::SOUND_NAME& _sndName)
{
	//元データがないときは警告
	if (effectRes_.find(_name) == effectRes_.end())assert("設定していないエフェクトを再生しようとしています。");

	if (_name == EFFECT_NAME::MAX)return-1;

	//要素の追加
	int addEfc = PlayEffekseer3DEffect(effectRes_[_name]);
	effectPlay_.push_back({ _name,addEfc });

	//各種設定同期
	SyncEffect(addEfc, _pos, _qua, _size, _speed);

	//効果音の再生
		if (_sndName != SoundManager::SOUND_NAME::MAX) {
			SoundManager::GetInstance().Play(_sndName);
		}

		return addEfc;
}

void EffectManager::StopOrderPlayHandle(const int _playHandle)
{
	//要素がない場合
	if (!IsEffectPlay(_playHandle)) {
		return;	//終了
	}

	//再生停止
	StopEffekseer3DEffect(_playHandle);
}

void EffectManager::StopOrderEffectName(const EFFECT_NAME& _name)
{
	//再生停止
	for (auto& data : effectPlay_) {
		if (data.name == _name) {
			StopEffekseer3DEffect(data.playHndle);
		}
	}
}

void EffectManager::StopAll(void)
{
	//再生停止
	for (auto& data : effectPlay_) {
		StopEffekseer3DEffect(data.playHndle);
	}
}

bool EffectManager::SyncEffect(const int _playHandle, const VECTOR& _pos, const Quaternion& _qua, const float& _size, const float& _speed)
{
	//存在しないものを同期させようとしたら
	if (!IsEffectPlay(_playHandle)) {
		return false;
	}

	//その他各種設定
	//大きさ
	SetScalePlayingEffekseer3DEffect(_playHandle, _size, _size, _size);
	//角度
	SetRotationPlayingEffekseer3DEffect(_playHandle, _qua.ToEuler().x, _qua.ToEuler().y, _qua.ToEuler().z);
	//位置
	SetPosPlayingEffekseer3DEffect(_playHandle, _pos.x, _pos.y, _pos.z);
	//速度
	SetSpeedPlayingEffekseer3DEffect(_playHandle, _speed);

	//正常終了
	return true;
}

void EffectManager::Update(void)
{
	std::vector<int>deleteEffectIdx;
	int idx = 0;

	//削除検索
	for (auto& data : effectPlay_) {
		//再生が終了していたら
		if (IsEffekseer3DEffectPlaying(data.playHndle) == -1) {
			deleteEffectIdx.push_back(idx);
		}
		idx++;
	}

	int deleteCount = 0;

	//削除
	for (auto& idx : deleteEffectIdx) {
		//実行
		effectPlay_.erase(effectPlay_.begin() + (idx - deleteCount));
		deleteCount++;
	}
}

void EffectManager::Release(void)
{
	for (auto& data : effectPlay_) {
		StopEffekseer3DEffect(data.playHndle);
	}

	//配列内の要素を全て消去
	//元々のデータはリソースマネージャが持っているのでおｋ。
	effectRes_.clear();
}

void EffectManager::Destroy(void)
{
	Release();
	delete instance_;
}

const bool EffectManager::IsEffectPlay(const int _playHandle)
{
	//内容確認
	for (auto& data : effectPlay_) {
		if (data.playHndle == _playHandle) {
			//発見
			return true;
		}
	}

	//要素なし
	return false;
}
