#include"../../pch.h"
#include"../../Application.h"
#include "AnimationController.h"

namespace {
	const float BLEND_RATE_MAX = 1.0f;	//ブレンド率の最大値
	const float BLEND_RATE_MIN = 0.0f;	//ブレンド率の最小値
	const float BLEND_RATE_ACC = 0.05f;	//ブレンド率の上昇量
	const AnimationController::AnimationSoundInfo SE_INFO_INIT	//アニメーションに合わせて再生するSEの情報の初期化
		= { SoundManager::SOUND_NAME::MAX, 0.0f, true };
	const AnimationController::AnimationInfo ANIMATION_INFO_INIT	//アニメーション情報の初期化
		= { L"",AnimationController::PLAY_TYPE::MAX, AnimationController::ANIM_SOURCE::MAX, -1, -1.0f, false ,false,SE_INFO_INIT };

	const AnimationController::AttachInfo ATTACH_INFO_INIT			//アタッチ情報の初期化
		= { -1,-1.0f,-1.0f,false };

	const AnimationController::FixAnimationAxis FIX_AXIS_INIT = { false,false,false };

	const float ANIMATION_RATE_MAX = 1.0f;
}

AnimationController::AnimationController(int& _model)
	:modelId_(_model)
	,animBlendRate_(BLEND_RATE_MIN)
	,currentAnim_(ANIMATION_INFO_INIT)
	,blendAnim_(ANIMATION_INFO_INIT)
	,isSetDefaultAnim_(false)
	,defaultAnim_(L"")
	,currentAnimAttachInfo_(ATTACH_INFO_INIT)
	,blendAnimAttachInfo_(ATTACH_INFO_INIT)
	,speedRate_(1.0f)
	,isAnimLock_(false)
	,finishAnim_(&AnimationController::FinishAnimNormal)
	,updateAnim_(&AnimationController::UpdateNormalAnim)
	,nextAnimList_({})
	,isFinishNormalAnim_(false)
	,isStartNextAnim_(false)
	,rootFrameIdx_(-1)
	,isUseFixPositionMethod_(false)
{
}

AnimationController::~AnimationController(void)
{
}

void AnimationController::Add(const std::wstring& _name, const int _animData, const PLAY_TYPE& _type, const ANIM_SOURCE& _source, const bool _isLock, const bool _isFixPosition)
{
	//すでに要素がある時
	if (animDatas_.contains(_name)) {
		assert("すでに登録しているものを再登録しようとしています。");
		return;	//再登録防止
	}


	//初期化(アタッチ番号だけ入れる)
	AnimationInfo anim = {};
	anim.name = _name;
	anim.type = _type;
	anim.source = _source;
	anim.data = _animData;
	anim.mustPlayOnce = _isLock;
	anim.isFixPosition = _isFixPosition;
	anim.seInfo = SE_INFO_INIT;

	if (_type == PLAY_TYPE::LOOP) {
		anim.mustPlayOnce = false;	//ループは再生保障の対象外
	}

	int attach = -1;
	SetAttachAnim(attach, anim.data, anim.source);	//アタッチ

	anim.total = MV1GetAttachAnimTotalTime(modelId_, attach);	//時間取得

	MV1DetachAnim(modelId_, attach);	//必要ないのでデタッチ
	animDatas_.emplace(_name, anim);	//アニメーション情報追加

	//補正情報の追加
	if (_isFixPosition) {
		fixAxisData_.emplace(_name, FIX_AXIS_INIT);
	}
}

void AnimationController::Play(const std::wstring& _name, const float _speed, const AnimationSoundInfo& _seInfo, const std::vector<NextAnimInfo> _next)
{
	//アニメーションロック中は再生しない
	if (isAnimLock_ || blendAnim_.mustPlayOnce) {
		AddNextAnim(_name, _speed, _seInfo);	//次のアニメーションに追加
		return;
	}

	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を再生しようとしています。");
		return;
	}

	//すでに再生が確定されている物の場合(現状も同様)
	if (currentAnim_.data == animDatas_[_name].data|| blendAnim_.data == animDatas_[_name].data) {
		return;	//処理を行わない
	}

	//まだアニメーションがないとき
	if (currentAnim_.data == -1) {
		SetAnimationPlayInfo(currentAnim_, animDatas_[_name], currentAnimAttachInfo_, _speed, _seInfo);	//再生情報の設定
		isAnimLock_ = animDatas_[_name].mustPlayOnce;	//再生保障
		SetFinishAndUpdateFunc();						//終了時と更新処理の設定
	}
	else {
		//現在ブレンド中のアニメーションがあるか
		if (blendAnim_.data != -1) {
			//ブレンド中のアニメーションを現在のアニメーションにする
			MV1DetachAnim(modelId_, currentAnimAttachInfo_.attachNum);	//現在のものをデタッチ
			currentAnim_ = blendAnim_;									//ブレンド中のものを現在のものに
			currentAnimAttachInfo_ = blendAnimAttachInfo_;				//ブレンドしているものを現在のものに
			isAnimLock_ = blendAnim_.mustPlayOnce;						//再生保障
		}

		SetAnimationPlayInfo(blendAnim_, animDatas_[_name], blendAnimAttachInfo_, _speed, _seInfo);	//新規のものをブレンド中のものに
	}
	
	
	//次に再生されるアニメーションが設定されているとき(LOOPは末尾のみ許可)
	if (!_next.empty()) {
		for (auto& anim : _next) {
			if (animDatas_[anim.name].type == PLAY_TYPE::LOOP && anim.name != _next.back().name) {
				assert("順次再生の個所を見直してください。");
			}
		}
		nextAnimList_ = _next;
	}
}

void AnimationController::ForcePlay(const std::wstring& _name, const float _speed, const AnimationSoundInfo& _seInfo, const std::vector<NextAnimInfo> _next)
{

	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を再生しようとしています。");
		return;
	}

	//すでに再生されようとしているものなら必要ない(Playとの違いは、被ブレンド対象も許容する点)
	if (blendAnim_.data == animDatas_[_name].data) {
		return;
	}

	if (blendAnim_.data != -1) {
		//ブレンド中のアニメーションを現在のアニメーションにする
		MV1DetachAnim(modelId_, currentAnimAttachInfo_.attachNum);	//現在のものをデタッチ
		currentAnim_ = blendAnim_;									//ブレンド中のものを現在のものに
		currentAnimAttachInfo_ = blendAnimAttachInfo_;				//ブレンドしているものを現在のものに
		isAnimLock_ = blendAnim_.mustPlayOnce;						//再生保障
	}
	SetAnimationPlayInfo(blendAnim_, animDatas_[_name], blendAnimAttachInfo_, _speed, _seInfo);	//新規のものをブレンド中のものに

	//割り込み前に設定されていたものを削除
	nextAnimList_.clear();

	//次に再生されるアニメーションが設定されているとき(LOOPは末尾のみ許可)
	if (!_next.empty()) {
		for (auto& anim : _next) {
			if (animDatas_[anim.name].type == PLAY_TYPE::LOOP && anim.name != _next.back().name) {
				assert("順次再生の個所を見直してください。");
			}
		}
		nextAnimList_ = _next;
	}
}

void AnimationController::NoBlendPlay(const std::wstring& _name, const float _speed, const AnimationSoundInfo& _seInfo, const std::vector<NextAnimInfo> _next)
{
	//アニメーションを直で設定
	SetAnimationPlayInfo(currentAnim_, animDatas_[_name], currentAnimAttachInfo_, _speed, _seInfo);	//再生情報の設定
	isAnimLock_ = animDatas_[_name].mustPlayOnce;	//再生保障
	SetFinishAndUpdateFunc();						//終了時と更新処理の設定

	//ブレンドがあった場合削除
	if (blendAnim_.data != -1) {
		MV1DetachAnim(modelId_, blendAnimAttachInfo_.attachNum);	//ブレンド中のものをデタッチ
		//ブレンド情報の初期化
		blendAnim_ = ANIMATION_INFO_INIT;
		blendAnimAttachInfo_ = ATTACH_INFO_INIT;
	}

	//割り込み前に設定されていたものを削除
	nextAnimList_.clear();

	//次に再生されるアニメーションが設定されているとき(LOOPは末尾のみ許可)
	if (!_next.empty()) {
		for (auto& anim : _next) {
			if (animDatas_[anim.name].type == PLAY_TYPE::LOOP && anim.name != _next.back().name) {
				assert("順次再生の個所を見直してください。");
			}
		}
		nextAnimList_ = _next;
	}
}

void AnimationController::AddNextAnim(const std::wstring& _name, const float _speed, const AnimationSoundInfo& _seInfo)
{
	//要素がないとき
	if (!animDatas_.contains(_name)) {
		//エラー防止
		assert("登録されていない要素を連続で再生しようとしています。");
		return;
	}

	if (animDatas_[_name].type == PLAY_TYPE::LOOP) {
		return;		//ループは連続再生に入れない
	}

	NextAnimInfo nextInfo;
	nextInfo.name = _name;
	nextInfo.speed = _speed;
	nextInfo.seInfo = _seInfo;

	nextAnimList_.push_back(nextInfo);	//順次再生リストに挿入
}

void AnimationController::AddNextAnim(const std::vector<NextAnimInfo> _animations)
{
	for (auto& add : _animations) {
		//要素がないとき
		if (!animDatas_.contains(add.name)) {
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

	isFinishNormalAnim_ = false;
	isStartNextAnim_ = false;

	bool isFix = (currentAnim_.isFixPosition || blendAnim_.isFixPosition) && rootFrameIdx_ != -1;

	//補正が必要な状態ならば
	if (isFix) {
		MV1ResetFrameUserLocalMatrix(modelId_, rootFrameIdx_);	//一度リセット
	}

	//カウンタ更新
	(this->*updateAnim_)();

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, currentAnimAttachInfo_.attachNum, currentAnimAttachInfo_.counter);
	if (blendAnimAttachInfo_.attachNum != -1) {
		MV1SetAttachAnimTime(modelId_, blendAnimAttachInfo_.attachNum, blendAnimAttachInfo_.counter);
	}

	//位置補正処理
	FixPosition();
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

const float AnimationController::GetCurrentAnimationProgressRate(void) const
{
	return currentAnimAttachInfo_.counter / currentAnim_.total;
}

const float AnimationController::GetBlendAnimationProgressRate(void) const
{
	if (blendAnimAttachInfo_.attachNum == -1) {
		return 0.0f;
	}
	return blendAnimAttachInfo_.counter / blendAnim_.total;
}

const float AnimationController::GetAnimTotalTime(const std::wstring& _name) const
{
	return animDatas_.at(_name).total;
}

void AnimationController::SetRootFrameIndex(const std::wstring& _frameName)
{
	//MV1SearchFrameのエラー時の返り値
	const int ERROR_SEARCH_CODE = -1;
	const int ERROR_SEARCH_CODE_2 = -2;

	int idx = MV1SearchFrame(modelId_, _frameName.c_str());

	//MV1SearchFrameのエラーコードの場合
	if (idx == ERROR_SEARCH_CODE || idx == ERROR_SEARCH_CODE_2) {
		return;	//処理しない
	}

	rootFrameIdx_ = idx;	//設定
}

void AnimationController::SetFixAnimationAxisInfo(const std::wstring _name, const bool _x, const bool _y, const bool _z)
{
	fixAxisData_.at(_name) = { _x,_y,_z };
}

void AnimationController::UpdateNormalAnim(void)
{
	currentAnimAttachInfo_.counter += currentAnimAttachInfo_.speed * speedRate_;	//カウンタ加算

	//まだSEが再生されていないとき
	if (!currentAnim_.seInfo.isPlayed) {
		if (currentAnim_.seInfo.playTiming <= GetCurrentAnimationProgressRate()) {
			SoundManager::GetInstance().Play(currentAnim_.seInfo.name);	//SE再生
			currentAnim_.seInfo.isPlayed = true;				//再生したフラグを立てる
		}
	}

	if (blendAnimAttachInfo_.attachNum != -1) {
		BlendAnim();
		blendAnimAttachInfo_.counter += blendAnimAttachInfo_.speed * speedRate_;	//カウンタ加算

		//まだSEが再生されていないとき
		if (!blendAnim_.seInfo.isPlayed) {
			if (blendAnim_.seInfo.playTiming <= GetBlendAnimationProgressRate()) {
				SoundManager::GetInstance().Play(blendAnim_.seInfo.name);	//SE再生
				blendAnim_.seInfo.isPlayed = true;				//再生したフラグを立てる
			}
		}
	}

	//再生上限にいった場合(まだアニメーションが終了していないとき→ブレンド中の対策)
	if (currentAnimAttachInfo_.counter > currentAnim_.total && !currentAnimAttachInfo_.isFinish) {
		(this->*finishAnim_)();	//アニメーション終了時処理
	}
}

void AnimationController::UpdateReturnAnim(void)
{
	currentAnimAttachInfo_.counter -= currentAnimAttachInfo_.speed * speedRate_;		//カウンタ減算（逆再生のため）

	//まだSEが再生されていないとき
	if (!currentAnim_.seInfo.isPlayed) {
		if ((ANIMATION_RATE_MAX - currentAnim_.seInfo.playTiming) >= GetCurrentAnimationProgressRate()) {
			SoundManager::GetInstance().Play(currentAnim_.seInfo.name);	//SE再生
			currentAnim_.seInfo.isPlayed = true;				//再生したフラグを立てる
		}
	}


	if (blendAnimAttachInfo_.attachNum != -1) {
		BlendAnim();
		blendAnimAttachInfo_.counter -= blendAnimAttachInfo_.speed * speedRate_;	//カウンタ加算

		//まだSEが再生されていないとき
		if (!blendAnim_.seInfo.isPlayed) {
			if ((ANIMATION_RATE_MAX - blendAnim_.seInfo.playTiming) >= GetBlendAnimationProgressRate()) {
				SoundManager::GetInstance().Play(blendAnim_.seInfo.name);	//SE再生
				blendAnim_.seInfo.isPlayed = true;				//再生したフラグを立てる
			}
		}
	}
	//再生上限にいった場合
	if (currentAnimAttachInfo_.counter <= 0.0f && !currentAnimAttachInfo_.isFinish){
		(this->*finishAnim_)();		//アニメーション終了時処理
	}
}

void AnimationController::FinishAnimNormal(void)
{
	//アニメーションロック解除
	isAnimLock_ = false;
	currentAnimAttachInfo_.isFinish = true;	//アニメーション終了

	isFinishNormalAnim_ = true;

	//次に再生されている物が設定されているとき
	if (!nextAnimList_.empty()) {
		//配列の最前列を再生
		Play(nextAnimList_[0].name, nextAnimList_[0].speed, nextAnimList_[0].seInfo);
		//要素の削除
		nextAnimList_.erase(nextAnimList_.begin());
		isStartNextAnim_ = true;
		return;
	}

	if(isSetDefaultAnim_) {
		//デフォルトアニメーションが設定されているときはそれを再生
		Play(defaultAnim_, DEFAULT_SPEED);
		nextAnimList_.clear();	//予約されているものを出し切ったため
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
	currentAnimAttachInfo_.isFinish = true;	//アニメーション終了
	currentAnimAttachInfo_.counter = currentAnim_.total;
}

void AnimationController::SetAnimationPlayInfo(AnimationInfo& _animInfo, const AnimationInfo& _sourceInfo, AttachInfo& _attachInfo, const float _animSpeed, const AnimationSoundInfo& _seInfo)
{
	_animInfo = _sourceInfo;	//アニメーション情報の設定
	_animInfo.seInfo = _seInfo;	//SE情報の設定
	SetAttachAnim(_attachInfo.attachNum, _animInfo.data, _animInfo.source);	//アタッチ
	_attachInfo.speed = _animSpeed;		//再生速度初期化
	_attachInfo.counter = 0.0f;			//カウンタ初期化
	_attachInfo.isFinish = false;		//終了フラグ初期
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
		finishAnim_ = &AnimationController::FinishAnimNormal;
		updateAnim_ = &AnimationController::UpdateNormalAnim;
		break;
	case PLAY_TYPE::LOOP:
		finishAnim_ = &AnimationController::FinishAnimLoop;
		updateAnim_ = &AnimationController::UpdateNormalAnim;
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
	isAnimLock_ = blendAnim_.mustPlayOnce;	//再生保障

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

void AnimationController::FixPosition(void)
{
	//親フレームが設定されていない場合
	if (rootFrameIdx_ == -1) {
		return;	//処理を行わない
	}

	if ((currentAnim_.isFixPosition || blendAnim_.isFixPosition)) {
		static const int MATRIX_MOVEMENT = 3;

		//補正を行う軸を取得
		const FixAnimationAxis useFixAxis = GetUseFixAnimationAxisData();
		
		//移動方向の打消し
		MATRIX localMat = MV1GetFrameLocalMatrix(modelId_, rootFrameIdx_);

		if (useFixAxis.x)localMat.m[MATRIX_MOVEMENT][0] = 0.0f;	//X
		if (useFixAxis.y)localMat.m[MATRIX_MOVEMENT][1] = 0.0f;	//Y
		if (useFixAxis.z)localMat.m[MATRIX_MOVEMENT][2] = 0.0f;	//Z
		MV1SetFrameUserLocalMatrix(modelId_, rootFrameIdx_, localMat);

		isUseFixPositionMethod_ = true;	//処理を行った
	}
	else if (isUseFixPositionMethod_) {
		//これまで補正を行っていたが、もう必要がなくなったとき
		MV1ResetFrameUserLocalMatrix(modelId_, rootFrameIdx_);	//通常に戻す

		isUseFixPositionMethod_ = false;	//処理を行わなかった
	}
}

const AnimationController::FixAnimationAxis AnimationController::GetUseFixAnimationAxisData(void) const
{
	FixAnimationAxis ret = FIX_AXIS_INIT;

	if (currentAnim_.isFixPosition) {
		FixAnimationAxis currentAnimFixAxisData = fixAxisData_.at(currentAnim_.name);

		if (currentAnimFixAxisData.x)ret.x = currentAnimFixAxisData.x;
		if (currentAnimFixAxisData.y)ret.y = currentAnimFixAxisData.y;
		if (currentAnimFixAxisData.z)ret.z = currentAnimFixAxisData.z;
	}
	if (blendAnim_.isFixPosition) {
		FixAnimationAxis blendAnimFixAxisData = fixAxisData_.at(blendAnim_.name);

		if (blendAnimFixAxisData.x)ret.x = blendAnimFixAxisData.x;
		if (blendAnimFixAxisData.y)ret.y = blendAnimFixAxisData.y;
		if (blendAnimFixAxisData.z)ret.z = blendAnimFixAxisData.z;
	}

	return ret;
}
