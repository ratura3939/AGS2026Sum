#include"../../pch.h"
#include"../../Application.h"
#include "AnimationController.h"

namespace {
	const float BLEND_RATE_MAX = 1.0f;	//ブレンド率の最大値
	const float BLEND_RATE_MIN = 0.0f;	//ブレンド率の最小値
	const float BLEND_RATE_ACC = 0.05f;	//ブレンド率の上昇量
	const AnimationController::AnimationInfo ANIMATION_INFO_INIT	//アニメーション情報の初期化
		= { AnimationController::PLAY_TYPE::MAX, AnimationController::ANIM_SOURCE::MAX, -1, -1.0f, false };

	const AnimationController::AttachInfo ATTACH_INFO_INIT			//アタッチ情報の初期化
		= { -1,-1.0f,-1.0f };
}

AnimationController::AnimationController(int& _model)
	:modelId_(_model)
	,animBlendRate_(BLEND_RATE_MIN)
	,currentAnim_(ANIMATION_INFO_INIT)
	, blendAnim_(ANIMATION_INFO_INIT)
	,isSetDefaultAnim_(false)
	,defaultAnim_(L"")
	,currentAnimAttachInfo_(ATTACH_INFO_INIT)
	,blendAnimAttachInfo_(ATTACH_INFO_INIT)
	,speedRate_(1.0f)
	,isAnimLock_(false)
	,finishAnim_(&AnimationController::FinishAnimNomal)
	,updateAnim_(&AnimationController::UpdateNomalAnim)
	,nextAnimList_({})
{
}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(const std::wstring& _name, const int _animData, const PLAY_TYPE& _type, const ANIM_SOURCE& _source, const bool _isLock)
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
	SetAttachAnim(attach, anim.data, anim.source);	//アタッチ

	anim.total = MV1GetAttachAnimTotalTime(modelId_, attach);	//時間取得

	MV1DetachAnim(modelId_, attach);	//必要ないのでデタッチ
	animDatas_.emplace(_name, anim);	//アニメーション情報追加
}

void AnimationController::Play(const std::wstring& _name, const float _speed, const std::vector<std::wstring> _next)
{
	//アニメーションロック中は再生しない
	if (isAnimLock_)return;

	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を再生しようとしています。");
		return;
	}

	//すでに再生が確定されている物の場合
	if (currentAnim_.data == animDatas_[_name].data|| blendAnim_.data == animDatas_[_name].data) {
		return;	//処理を行わない
	}

	//まだアニメーションがないとき
	if (currentAnim_.data == -1) {
		SetAnimationPlayInfo(currentAnim_, animDatas_[_name], currentAnimAttachInfo_, _speed);	//再生情報の設定
		isAnimLock_ = animDatas_[_name].mustPlayOnce;	//再生保障
		SetFinishAndUpdateFunc();	//終了時と更新処理の設定
	}
	else {
		//現在ブレンド中のアニメーションがあるか
		if (blendAnim_.data != -1) {
			//ブレンド中のアニメーションを現在のアニメーションにする
			MV1DetachAnim(modelId_, currentAnimAttachInfo_.attachNum);	//現在のものをデタッチ
			currentAnim_ = blendAnim_;				//ブレンド中のものを現在のものに
		}

		SetAnimationPlayInfo(blendAnim_, animDatas_[_name], blendAnimAttachInfo_, _speed);	//新規のものをブレンド中のものに
	}
	
	
	//次に再生されるアニメーションが設定されているとき(LOOPは末尾のみ許可)
	if (!_next.empty()) {
		for (auto& wstring : _next) {
			if (animDatas_[wstring].type == PLAY_TYPE::LOOP && wstring != _next.back()) {
				assert("順次再生の個所を見直してください。");
			}
		}
		nextAnimList_ = _next;
	}
}

void AnimationController::AddNextAnim(const std::wstring& _name)
{
	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を連続で再生しようとしています。");
		return;
	}
	nextAnimList_.push_back(_name);
}

void AnimationController::AddNextAnim(const std::vector<std::wstring> _names)
{
	for (auto& add : _names) {
		//要素がないとき
		if (!animDatas_.contains(add)) {
			//エラー防止
			assert("登録されていない要素を連続で再生しようとしています。");
			return;
		}
		nextAnimList_.push_back(add);
	}
}

void AnimationController::Update(void)
{
	//初期値のとき
	if (currentAnimAttachInfo_.attachNum == -1)return;

	//カウンタ更新
	(this->*updateAnim_)();

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, currentAnimAttachInfo_.attachNum, currentAnimAttachInfo_.counter);
	if (blendAnimAttachInfo_.attachNum != -1) {
		MV1SetAttachAnimTime(modelId_, blendAnimAttachInfo_.attachNum, blendAnimAttachInfo_.counter);
	}
}

void AnimationController::ChangeSpeedRate(const float _percent)
{
 	speedRate_ = _percent / DEFAULT_SPEED_RATE;
}

void AnimationController::SetDefaultAnim(const std::wstring& _name)
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
	currentAnimAttachInfo_.counter += currentAnimAttachInfo_.speed * speedRate_;	//カウンタ加算

	if (blendAnimAttachInfo_.attachNum != -1) {
		BlendAnim();
		blendAnimAttachInfo_.counter += blendAnimAttachInfo_.speed * speedRate_;	//カウンタ加算
	}
	//再生上限にいった場合
	if (currentAnimAttachInfo_.counter > currentAnim_.total){
		(this->*finishAnim_)();	//アニメーション終了時処理
	}
}

void AnimationController::UpdateReturnAnim(void)
{
	currentAnimAttachInfo_.counter -= currentAnimAttachInfo_.speed * speedRate_;		//カウンタ減算（逆再生のため）

	if (blendAnimAttachInfo_.attachNum != -1) {
		BlendAnim();
		blendAnimAttachInfo_.counter -= blendAnimAttachInfo_.speed * speedRate_;	//カウンタ加算
	}
	//再生上限にいった場合
	if (currentAnimAttachInfo_.counter <= 0.0f){
		(this->*finishAnim_)();		//アニメーション終了時処理
	}
}

void AnimationController::FinishAnimNomal(void)
{
	//アニメーションロック解除
	isAnimLock_ = false;

	//次に再生されている物が設定されているとき
	if (!nextAnimList_.empty()) {
		//配列の最前列を再生
		Play(nextAnimList_[0], currentAnimAttachInfo_.speed);
		//要素の削除
		nextAnimList_.erase(nextAnimList_.begin());
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
	currentAnimAttachInfo_.counter = 0.0f;
}

void AnimationController::FinishAnimReturn(void)
{
	//アニメーションロック解除
	isAnimLock_ = false;
	currentAnimAttachInfo_.counter = currentAnim_.total;
}

void AnimationController::SetAnimationPlayInfo(AnimationInfo& _animInfo, const AnimationInfo& _sourceInfo, AttachInfo& _attachInfo, const float _animSpeed)
{
	_animInfo = _sourceInfo;	//アニメーション情報の設定
	SetAttachAnim(_attachInfo.attachNum, _animInfo.data, _animInfo.source);	//アタッチ
	_attachInfo.speed = _animSpeed;		//再生速度初期化
	_attachInfo.counter = 0.0f;			//カウンタ初期化
	//逆再生のときはカウンタを総再生時間にする
	if (_animInfo.type == PLAY_TYPE::RETURN) {
		_attachInfo.counter = _animInfo.total;
	}
	MV1SetAttachAnimTime(modelId_, _attachInfo.attachNum, _attachInfo.counter);	//再生するアニメーション時間の設定
}


void AnimationController::SetFinishAndUpdateFunc(void)
{
	//終了時処理の設定
	switch (currentAnim_.type)
	{
	case PLAY_TYPE::NORMAL:
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
}

void AnimationController::BlendAnim(void)
{
	MV1SetAttachAnimBlendRate(modelId_, currentAnimAttachInfo_.attachNum, BLEND_RATE_MAX - animBlendRate_);	//ブレンド率の設定
	MV1SetAttachAnimBlendRate(modelId_, blendAnimAttachInfo_.attachNum, animBlendRate_);					//ブレンド率の設定

	animBlendRate_ += BLEND_RATE_ACC;	//ブレンド率加算

	//上限またはアニメーション終了時
	if (animBlendRate_ > BLEND_RATE_MAX/* || blendAnimAttachInfo_.counter >= blendAnim_.total*/) {
		FinishBlendAnim();	//ブレンド終了処理
	}
}

void AnimationController::FinishBlendAnim(void)
{
	MV1DetachAnim(modelId_, currentAnimAttachInfo_.attachNum);			//古いアニメーションをデタッチ
	currentAnimAttachInfo_ = blendAnimAttachInfo_;	//ブレンドしているものを現在のものに
	currentAnim_ = blendAnim_;						//ブレンドしているものを現在のものに

	SetFinishAndUpdateFunc();				//終了時と更新処理の設定

	blendAnim_ = ANIMATION_INFO_INIT;		//ブレンド中のアニメーション情報初期化
	blendAnimAttachInfo_ = ATTACH_INFO_INIT;//ブレンド中のアタッチ初期化
	animBlendRate_ = BLEND_RATE_MIN;		//ブレンド率初期化
}

void AnimationController::SetAttachAnim(int& _attachAnim, const int _animData, const ANIM_SOURCE& _source)
{
	if (_source == ANIM_SOURCE::EMBEDDED) {
		_attachAnim = MV1AttachAnim(modelId_, _animData);
	}
	else if (_source == ANIM_SOURCE::EXTERNAL) {
		_attachAnim = MV1AttachAnim(modelId_, 0, _animData, true);
	}
	else {
		assert("アニメーション登録でエラーが起きています");
	}
}
