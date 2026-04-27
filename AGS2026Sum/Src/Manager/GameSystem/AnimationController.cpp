#include"../../pch.h"
#include"../../Application.h"
#include "AnimationController.h"

AnimationController::AnimationController(int& _model):modelId_(_model)
{
	activeAnim_.type=PLAY_TYPE::MAX;
	activeAnim_.source = ANIM_SOURCE::MAX;
	activeAnim_.data = -1;
	activeAnim_.total = -1.0f;

	isSetDefaultAnim_ = false;
	defaultAnim_ = "";

	attachAnim_ = -1;
	speedAnim = -1.0f;
	counter_ = -1.0f;
	speedRate_ = 1.0f;

	isAnimLock_ = false;

	finishAnim_ = &AnimationController::FinishAnimNomal;
	updateAnim_ = &AnimationController::UpdateNomalAnim;

	nextAnim_ = {};
}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(const std::string& _name, const int _animData, const PLAY_TYPE& _type, const ANIM_SOURCE& _source, const bool _isLock)
{
	//すでに要素がある時
	if (animDatas_.contains(_name)) {
		assert("すでに登録しているものを再登録しようとしています。");
		return;	//再登録防止
	}


	//初期化(アタッチ番号だけ入れる)
	AnimationInfo anim = {};
	anim.type = _type;
	anim.source = _source;
	anim.data = _animData;
	anim.mustPlayOnce = _isLock;

	if (_type == PLAY_TYPE::LOOP) {
		anim.mustPlayOnce = false;	//ループは再生保障の対象外
	}

	int attach = -1;

	//アタッチ（EMBEDDED：第２引数、EXTERNAL：第３引数にdataを渡す）
	if (anim.source == ANIM_SOURCE::EMBEDDED) {
		attach = MV1AttachAnim(modelId_, anim.data);
	}
	else if (anim.source == ANIM_SOURCE::EXTERNAL) {
		attach = MV1AttachAnim(modelId_, 0, anim.data, true);
	}
	else {
		assert("アニメーション登録でエラーが起きています");
	}

	anim.total = MV1GetAttachAnimTotalTime(modelId_, attach);	//時間取得

	MV1DetachAnim(modelId_, attach);	//必要ないのでデタッチ
	animDatas_.emplace(_name, anim);	//アニメーション情報追加
}

void AnimationController::Play(const std::string& _name, const float _speed, const std::vector<std::string> _next)
{
	//アニメーションロック中は再生しない
	if (isAnimLock_)return;

	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を再生しようとしています。");
		return;
	}

	//もしかしたらこのifの中身が追加条件はいるかも
	//現在アタッチしているものと同じものなら処理は行わない
	if (activeAnim_.data == animDatas_[_name].data)return;

	//初期値以外のとき
	if (attachAnim_ != -1) {
		MV1DetachAnim(modelId_, attachAnim_);	//現在のものをデタッチ
	}
	
	//次に再生されるアニメーションが設定されているとき(LOOPは末尾のみ許可)
	if (!_next.empty()) {
		for (auto& string : _next) {
			if (animDatas_[string].type == PLAY_TYPE::LOOP && string != _next.back()) {
				assert("順次再生の個所を見直してください。");
			}
		}
		nextAnim_ = _next;
	}

	//新規を代入
	activeAnim_.type = animDatas_[_name].type;
	activeAnim_.source = animDatas_[_name].source;
	activeAnim_.data = animDatas_[_name].data;
	activeAnim_.total = animDatas_[_name].total;
	
	//アタッチ（EMBEDDED：第２引数、EXTERNAL：第３引数にdataを渡す）
	if (animDatas_[_name].source == ANIM_SOURCE::EMBEDDED) {
		attachAnim_ = MV1AttachAnim(modelId_, activeAnim_.data);
	}
	else if (animDatas_[_name].source == ANIM_SOURCE::EXTERNAL) {
		attachAnim_ = MV1AttachAnim(modelId_, 0, activeAnim_.data, true);
	}
	else {
		assert("アニメーション登録でエラーが起きています");
	}

	//終了時処理の設定
	switch (activeAnim_.type)
	{
	case PLAY_TYPE::NOMAL:
		finishAnim_ = &AnimationController::FinishAnimNomal;
		updateAnim_ = &AnimationController::UpdateNomalAnim;
		break;
	case PLAY_TYPE::LOOP:
		finishAnim_ = &AnimationController::FinishAnimLoop;
		updateAnim_ = &AnimationController::UpdateNomalAnim;
		break;
	case PLAY_TYPE::RETURN:
		finishAnim_ = &AnimationController::FinishAnimReturn;
		updateAnim_ = &AnimationController::UpdateReturnAnim;
		break;
	default:
		assert("アニメーション登録でエラーが起きています");
		break;
	}

	speedAnim = _speed;
	counter_ = 0.0f;

	//逆再生時の初期化
	if (activeAnim_.type == PLAY_TYPE::RETURN) {
		counter_ = activeAnim_.total;
	}

	//再生保障
	isAnimLock_= animDatas_[_name].mustPlayOnce;

	//再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, attachAnim_, counter_);
}

void AnimationController::AddNextAnim(const std::string& _name)
{
	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を連続で再生しようとしています。");
		return;
	}
	nextAnim_.push_back(_name);
}

void AnimationController::AddNextAnim(const std::vector<std::string> _names)
{
	for (auto& add : _names) {
		//要素がないとき
		if (!animDatas_.contains(add)) {
			//エラー防止
			assert("登録されていない要素を連続で再生しようとしています。");
			return;
		}
		nextAnim_.push_back(add);
	}
}

void AnimationController::Update(void)
{
	//初期値のとき
	if (attachAnim_ == -1)return;

	//カウンタ更新
	(this->*updateAnim_)();

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, attachAnim_, counter_);
}

void AnimationController::ChangeSpeedRate(const float _percent)
{
 	speedRate_ = _percent / DEFAULT_SPEED_RATE;
}

void AnimationController::SetDefaultAnim(const std::string& _name)
{
	//要素がないとき
	if (!animDatas_.contains(_name)) {
		return;	//エラー防止
	}
	if (animDatas_[_name].type != PLAY_TYPE::LOOP) {
		return;	//ループ以外は設定できない
	}
	isSetDefaultAnim_ = true;
	defaultAnim_ = _name;
}

void AnimationController::UpdateNomalAnim(void)
{
	counter_ += speedAnim * speedRate_;	//カウンタ加算
	//再生上限にいった場合
	if (counter_ > activeAnim_.total)
	{
		(this->*finishAnim_)();	//アニメーション終了時処理
	}
}

void AnimationController::UpdateReturnAnim(void)
{
	counter_ -= speedAnim * speedRate_;		//カウンタ減算（逆再生のため）
	//再生上限にいった場合
	if (counter_ <= 0.0f)
	{
		(this->*finishAnim_)();		//アニメーション終了時処理
	}
}

void AnimationController::FinishAnimNomal(void)
{
	//アニメーションロック解除
	isAnimLock_ = false;

	//次に再生されている物が設定されているとき
	if (!nextAnim_.empty()) {
		//配列の最前列を再生
		Play(nextAnim_[0], speedAnim);
		//要素の削除
		nextAnim_.erase(nextAnim_.begin());
		return;
	}

	if(isSetDefaultAnim_) {
		//デフォルトアニメーションが設定されているときはそれを再生
		Play(defaultAnim_, DEFAULT_SPEED);
		return;
	}
}

void AnimationController::FinishAnimLoop(void)
{
	//アニメーションロック解除
	isAnimLock_ = false;
	counter_ = 0.0f;
}

void AnimationController::FinishAnimReturn(void)
{
	//アニメーションロック解除
	isAnimLock_ = false;
	counter_ = activeAnim_.total;
}
