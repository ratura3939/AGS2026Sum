#include"../../pch.h"
#include"../../Object/Character/Enemy/EnemyManager.h"
#include"../../Manager/GameSystem/GravityManager.h"
#include"../../Manager/GameSystem/AttackManager.h"
#include"../../Manager/GameSystem/CollisionManager.h"
#include"../../Manager/GameSystem/ChunkManager.h"
#include"../../Manager/GameSystem/ComboManager.h"
#include"../../Manager/GameSystem/Event/EventManager.h"
#include"../../Manager/Generic/Camera.h"
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/ResourceManager.h"
#include"../../Manager/Decoration/SoundManager.h"
#include"../../Manager/Decoration/EffectManager.h"
#include"../../Manager/Decoration/UIManager2d.h"
#include"../../Object/Character/Player/PlayerManager.h"
#include"../../Object/Stage/StageManager.h"
#include "../../Scene/Sub/PauseScene.h"
#include"../../Utility/Utility.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include"../../Application.h"
#include "Title.h"
#include "GameClear.h"
#include "GameOver.h"
#include "Game.h"

//ローカル定数
namespace {
	constexpr VECTOR CAMERA_START_1 = { 600.0f,200.0f,0.0f };	//カメラ演出開始位置
	constexpr VECTOR CAMERA_GOAL_1 = { 600.0f,1000.0f,0.0f };	//カメラ演出目標位置その①
	constexpr VECTOR CAMERA_GOAL_2 = { 0.0f,800.0f,600.0f };	//カメラ演出目標位置その②
	constexpr float ALLOWABLE_DISTANCE = 10.0f;		//カメラの移動完了判定をがば目にするために
	constexpr int BOSS_IDX = 0;		//ボスの配列番号(ボス単体のため必ず0)


	const int LIMIT_SLOW = 120;					//スロー演出時間
	const int BGM_VOL_MAX = 100;				//BGM音量最大値
	const int BGM_VOL_ACC = 1;					//BGM切り換えスピード
	const float NORMAL_SPEED_PERCENT = 100.0f;	//通常の割合
	const float SLOW_SPEED_PERCENT = 10.0f;		//スローの割合
		  
	const int WARNING_DIRECTION_TIME = 150;		//WARNING警告時間
	const int CAMERA_SHAKE_NUM = 3;				//カメラ演出における振動回数
	const int CAMERA_SHAKE_COOL_TIME = 40;		//振動のクールタイム

	const float CAMERA_FOLLOW_DIFF_Y_ABILITY = 200.0f;	//能力使用時の注視点差分

	const float LOCK_DISTANCE_MIN_NOMAL = 500.0f;		//ロックオン時に最低限離れておく距離
	const float LOCK_DISTANCE_MIN_BOSS = 1000.0f;		//ロックオン時に最低限離れておく距離

	const int BGM_VOL = 80;

	const int PS_EDGE_BUFF_NUM = 2;	//エッジ描画用のバッファ数
	const float EDGE_NORMAL_THRESHOLD = 0.7f;	//エッジ描画用の閾値
	const float EDGE_DEPTH_THRESHOLD = 0.7f;	//エッジ描画用の閾値

	const int BGM_VOLUME_PERCENT = 90;	//BGM音量

	//現在の進行度
	std::array<std::string, static_cast<int>(Game::GAME_PROGRESS::MAX)> PROGRESS =
	{
		"Tutorial"
		,"Stage1"
	};
}

Game::Game(void)
{
	isSlowEffect_ = false;
	slowCnt_ = -1;
	nextBgmVol_ = 0;
	switchBgm_ = false;

	prevInputP_ = false;
	isEnemyUpdate_ = true;

	cameraMoveStartPos_ = CAMERA_START_1;
	cameraMoveGoalPos_[0] = CAMERA_GOAL_1;
	cameraMoveGoalPos_[1] = CAMERA_GOAL_2;
	direcState_ = BOSS_DIRECTION::NONE;
	cameraShakeCollTimeCnt_ = 0;
	stayCameraShake_ = false;

	progress_ = GAME_PROGRESS::TUTORIAL;

	isDrawPostEffect_ = false;
	direcCnt_ = 0;

	update_ = &Game::GameUpdate;
	//drawPostEffect_ = &Game::GameUpdate;

	cameraGoalStayCounter_ = 0;
	cameraGoalStayTime_ = 0;

	isDebug_ = false;
}

Game::~Game(void)
{
	//DeleteGraph(scanLineScreen_);
	//DeleteGraph(blurScreen_);
}

void Game::Init(void)
{
	//リソース準備
	ResourceManager& rsM = ResourceManager::GetInstance();
	rsM.GetInstance().Init(SceneManager::SCENE_ID::GAME);

	update_ = &Game::GameUpdate;

	//進行度
	progress_ = GAME_PROGRESS::TUTORIAL;

	//更新
	updateProgress_ = &Game::UpdateTutorial;

	//生成

	//重力マネージャー
	GravityManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::GAME_END);

	//攻撃マネージャー
	AttackManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::GAME_END);
	
	//チャンク管理
	ChunkManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::GAME_END);

	//コンボ管理
	ComboManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//プレイヤー
	player_ = std::make_unique<PlayerManager>(*this);
	player_->Init();

	//ステージごとの敵情報
	stageEnemyData_ = rsM.Load(ResourceManager::SRC::STAGE_ENEMY_DATA).GetData<StageEnemyData>();

	//敵
	enemy_ = std::make_unique<EnemyManager>(player_->GetPos());
	enemy_->Load();
	enemy_->Init();
	enemy_->CreateStageEnemy(stageEnemyData_.allStageInfo[PROGRESS[static_cast<int>(progress_)]]);

	//ステージ
	stage_ = std::make_unique<StageManager>();
	stage_->Load();
	stage_->Init();

	//カメラの初期設定
	Camera& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
	camera.SetGoalFocusPos(player_->GetFocusPos());				//注視点
	camera.SetLockOnDistanceMin(LOCK_DISTANCE_MIN_NOMAL);		//ロックオン最低距離

	//音関係初期設定
	InitSound();
	//エフェクト関係初期化
	InitEffect();
	//シェーダー初期化
	InitShader();

	auto& uiM = UIManager2d::GetInstance();

	//メニューボタン
	//uiM.Add(MENU_BTN, rsM.Load(ResourceManager::SRC::MENU_BTN).handleId_, UIManager2d::UI_DIRECTION_2D::NORMAL, UIManager2d::UI_DRAW_DIMENSION::DIMENSION_2);
	//uiM.SetUIInfo(MENU_BTN, VECTOR{ static_cast<float>(Application::SCREEN_SIZE_X - BTN_DIFF_X),static_cast<float>(Application::SCREEN_SIZE_Y - BTN_DIFF_Y),0.0f }, BTN_EX);
}

void Game::InitSound(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();

	using SND_TYPE = SoundManager::TYPE;
	using SND_NAME = SoundManager::SOUND_NAME;

	//BGM
	sndM.Add(SND_TYPE::BGM, SND_NAME::GAME_BGM,
		rsM.Load(ResourceManager::SRC::GAME_BGM).handleId_);

	sndM.AdjustVolume(SND_NAME::GAME_BGM, BGM_VOLUME_PERCENT);

	sndM.Play(SND_NAME::GAME_BGM);
}

void Game::InitEffect(void)
{
	ResourceManager& rsM = ResourceManager::GetInstance();
	EffectManager& efcM = EffectManager::GetInstance();

	//剣
	//efcM.Add(EffectManager::EFFECT_NAME, rsM.Load(ResourceManager::SRC::SWORD_EFC).handleId_);
}

void Game::InitShader(void)
{
	Application& app = Application::GetInstance();
	int screemWidth = app.GetWindowWidth();
	int screemHeight = app.GetWindowHeight();

	normalDepthScreen_ = MakeScreen(screemWidth, screemHeight, true);

	edgeMaterial_ = std::make_unique<PixelMaterial>(L"EdgeDetectPS.cso", PS_EDGE_BUFF_NUM);
	edgeMaterial_->AddConstBuf(FLOAT4{ 0.0f,0.0f,0.0f,1.0f });	//エッジの色
	edgeMaterial_->AddConstBuf(FLOAT4{ 1.0f / static_cast<float>(screemWidth),1.0f / static_cast<float>(screemHeight),EDGE_DEPTH_THRESHOLD,EDGE_NORMAL_THRESHOLD });	//エッジの色
	edgeMaterial_->AddTextureBuf(normalDepthScreen_);	//法線・深度描画用スクリーンをテクスチャとして登録


	edgeRender_ = std::make_unique<PixelRenderer>();
	edgeRender_->MakeSquereVertex({ 0,0 }, { screemWidth, screemHeight });
}

void Game::Update(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	Camera& camera = scM.GetCamera();
	InputManager& inpM = InputManager::GetInstance();

#pragma region シーン遷移(ルール)
	//プレイヤーが死んでいたら
	//if (!player_->IsAlive()) {
	//	//BGM念のため両方停止
	//	sndM.Stop(nowBgmStr_);
	//	sndM.Stop(switchBgmStr_);
	//	//シーン遷移
	//	//scM.ChangeScene(std::make_shared<GameOver>());
	//}


	//if(enemy_->GetActiveEnemyNum() <= 0)
	//{
	//	SceneManager::GetInstance().ChangeScene(std::make_shared<GameClear>());
	//}
	if (!player_->IsAlive())
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<GameOver>());
	}

#pragma endregion

	//更新
	(this->*update_)();

	//カメラ状態
	float cameraPosToGoalDiff = Utility::MagnitudeF(VSub(camera.GetGoalPos(), camera.GetPos()));	//現在位置と目標位置までの距離

	//一定の距離以内だったら
	if (cameraPosToGoalDiff <= ALLOWABLE_DISTANCE) {
		cameraGoalStayCounter_++;

		//一定時間経過していたら
		if (cameraGoalStayCounter_ >= cameraGoalStayTime_) {
			//フォローに変化
			camera.ChangeMode(Camera::MODE::FOLLOW);		//追従に変更(リセット後のモード変更用)
			camera.SetFocusPos(player_->GetFocusPos());		//注視点
			EndSlow();		//スロー演出終了
			cameraGoalStayCounter_ = 0;

			camera.ChangeMode(Camera::MODE::RESET);	//カメラリセット
		}
	}
}

void Game::GameUpdate(void)
{
	SceneManager& scM = SceneManager::GetInstance();
	SoundManager& sndM = SoundManager::GetInstance();
	InputManager& inpM = InputManager::GetInstance();
	Camera& camera = scM.GetCamera();
	
	//敵がいなくなったら
	//if (enemy_->GetEnemys().size() <= 0) {
	//	sndM.Stop(nowBgmStr_);
	//	sndM.Stop(switchBgmStr_);
	//	//シーン遷移
	//	//scM.ChangeScene(std::make_shared<GameClear>());
	//}

	//ポーズシーン遷移
	//if (inpM.IsTrigerrDown(InputManager::INPUT_COMMAND::PAUSE)) {
	//	//シーン追加(一つ次へ)
	//	scM.PushScene(std::make_shared<PauseScene>());
	//}

#pragma region 基礎アプデ
	stage_->Update();

	//プレイヤー
	player_->Update();
	//敵はスローの効果を受ける
	if (isSlowEffect_) {
		//スロー時の更新(このカウンタはスローの影響を受けない)
		slowCnt_++;
		if (slowCnt_ >= LIMIT_SLOW) {
			EndSlow();
		}
	}

	//敵
	if (isEnemyUpdate_) {
		enemy_->Update();
	}

	//当たり判定更新
	CollisionManager::GetInstance().UpdateColliders();

	//コンボの更新
	ComboManager::GetInstance().Update();

	//進行度の更新
	(this->*updateProgress_)();

#pragma endregion

#pragma region BGM
	//敵の状態(戦闘・それ以外)のトリガ
	//if (enemy_->IsBattleStateChanged()) {
	//	//もともと切り換え中だったら
	//	if (switchBgm_) {
	//		//強制終了処理
	//		FinishSwitchBgm();
	//	}

	//	//切り換え開始
	//	switchBgm_ = true;
	//	//切り替え後の再生
	//	sndM.Play(switchBgmStr_);
	//}

	//BGM切り換え実行中
	if (switchBgm_) {
		//音量調整に加算
		nextBgmVol_ += BGM_VOL_ACC;
		//sndM.AdjustVolume(switchBgmStr_(SOUND_NAME), nextBgmVol_);			//次のBGMは音量をあげる
		//sndM.AdjustVolume(nowBgmStr_(SOUND_NAME), (BGM_VOL_MAX - nextBgmVol_));	//現在のBGMは音量を下げる

		//もしボリュームが最大値以上なら
		if (nextBgmVol_ >= BGM_VOL_MAX) {
			//音量を最大値に
			nextBgmVol_ = BGM_VOL_MAX;	
			//終了処理
			FinishSwitchBgm();
		}
	}
#pragma endregion

#pragma region カメラ
	//カメラの設定
	//if (!player_->IsUseAbility()) {
	//	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象の更新
	//}
	//else {
	//	VECTOR abilityFollow = player_->GetPos();
	//	abilityFollow.y += CAMERA_FOLLOW_DIFF_Y_ABILITY;
	//	camera.SetFollow(abilityFollow, player_->GetQua());		//追従対象の更新
	//}

	camera.SetFollow(player_->GetFocusPos(), player_->GetQua());		//追従対象の更新
	
#pragma endregion
}

void Game::DirectionUpdate(void)
{
	//危険のポストエフェクト→画面揺れ→カメラ
	if ((this->*direcUpdate_)()) {
		//次の演出に
		direcState_ = static_cast<BOSS_DIRECTION>(static_cast<int>(direcState_) + 1);
		direcCnt_ = 0;

		//もし終了したら
		//if (direcState_ == BOSS_DIRECTION::END) {
		//	//カメラの追従対象を戻す
		//	Camera& camera = SceneManager::GetInstance().GetCamera();
		//	camera.ChangeMode(Camera::MODE::FOLLOW);					//モード選択
		//	camera.SetFollow(player_->GetPos(), player_->GetQua());		//追従対象
		//	camera.SetGoalFocusPos(player_->GetFocusPoint());				//注視点

		//	//ブラーをなくす
		//	ChangeActionDirec(ACTION_DIRECTION::NOMAL);

		//	//BGM流す
		//	SoundManager::GetInstance().Play("BossBgm");
		//	nowBgmStr_ = "BossBgm";
		//	switchBgm_ = false;

		//	//更新を通常に
		//	update_ = &Game::GameUpdate;
		//}
		////画面揺れ
		//else if (direcState_ == BOSS_DIRECTION::SHAKE_SCREEN) {
		//	//実行
		//	DoShake();
		//	//演出の更新を「画面揺れ」に
		//	direcUpdate_ = &Game::DirectionShakeScreen;
		//}
		////カメラ移動
		//else if (direcState_ == BOSS_DIRECTION::CAMERA_MOVE) {

		//	auto& camera = SceneManager::GetInstance().GetCamera();
		//	//ボスの生成
		//	enemy_->CreateBoss(player_->GetPos());
		//	camera.SetLockOnDistanceMin(LOCK_DISTANCE_MIN_BOSS);			//ロックオン最低距離

		//	//カメラを自動移動に設定
		//	camera.ChangeMode(Camera::MODE::AUTO_MOVE);

		//	//場所の設定(ボスの横ぐらい)
		//	auto bossPos = enemy_->GetPos(BOSS_IDX);
		//	camera.SetPos(VAdd(bossPos,cameraMoveStartPos_), bossPos);
		//	camera.SetGoalPos(VAdd(bossPos, cameraMoveGoalPos_[direcCnt_]));

		//	//演出を「カメラ移動に変更
		//	direcUpdate_ = &Game::DirectionCameraMove;
		//}
	}
}

bool Game::DirectionPostEffect(void)
{
	////WARNING更新
	//UIManager2d::GetInstance().Update(WARNING_STR_IMG);

	////ポストエフェクト更新
	//scanLineMaterial_->SetConstBuf(1, { SceneManager::GetInstance().GetTotalTime(),0.0f,0.0f,0.0f });	//横ライン移動用

	////演出カウンタ更新
	//direcCnt_++;
	////一定時間過ぎたら
	//if (direcCnt_ > WARNING_DIRECTION_TIME) {
	//	//演出終了
	//	SoundManager::GetInstance().Stop("WarningBgm");	//警告音止める
	//	ChangeActionDirec(ACTION_DIRECTION::NOMAL);		//ポストエフェクト終了
	//	return true;
	//}
	////演出が続く
	return false;
}

bool Game::DirectionShakeScreen(void)
{
	//カメラノーシェイク時
	if (stayCameraShake_) {
		//クールタイム増加
		cameraShakeCollTimeCnt_++;

		//一定時間経過後
		if (cameraShakeCollTimeCnt_ >= CAMERA_SHAKE_COOL_TIME) {
			//再度揺らす
			DoShake();
			stayCameraShake_ = false;
		}
		return false;
	}

	//カメラシェイク終了時
	if (SceneManager::GetInstance().GetCamera().IsFinishShake()) {
		//演出カウンタ増加
		direcCnt_++;

		//一定数行ったら
		if (direcCnt_ >= CAMERA_SHAKE_NUM) {
			//演出終了
			return true;
		}
		//クールタイム関係リセット
		cameraShakeCollTimeCnt_ = 0;
		stayCameraShake_ = true;
	}
	//演出が続く
	return false;
}

void Game::DoShake(void)
{
	//oundManager::GetInstance().Play("Impact");	//効果音再生
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::SHAKE);	//揺らす
}

bool Game::DirectionCameraMove(void)
{
	//アニメーションのみ更新
	//enemy_->UpdateAnim();

	//カメラ演出用
	auto& camera = SceneManager::GetInstance().GetCamera();

	//ゴール位置についたら次のスタート位置へ
	auto cameraPos = camera.GetPos();
	if (Utility::MagnitudeF(VSub(camera.GetGoalPos(), cameraPos)) <= ALLOWABLE_DISTANCE) {
		//演出カウンタ増加
		direcCnt_++;

		//移動演出回数の上限に到達していたら
		if (direcCnt_ >= CAMERA_DIRECTION_NUM) {
			//演出終了
			return true;
		}
		else {
			//次の目標地点への設定
			//camera.SetGoalPos(VAdd(enemy_->GetPos(BOSS_IDX), cameraMoveGoalPos_[direcCnt_]));

			//二回目の移動はボスの「叫び」も入れる
			//enemy_->BossShout();
			//「叫び」演出用のブラーへ
			ChangeActionDirec(ACTION_DIRECTION::BLUR);
		}
	}
	//演出が続く
	return false;
}

void Game::DrawEdge(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	//法線・深度描画用スクリーンに描画
	SetDrawScreen(normalDepthScreen_);

	ClearDrawScreen();

	SceneManager::GetInstance().GetCamera().SetBeforeDraw();
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	player_->DrawNormalDepth();

	// メインに戻す
	SetDrawScreen(mainScreen);

	edgeMaterial_->SetTextureBuf(0, normalDepthScreen_);
	edgeRender_->Draw(*edgeMaterial_);
}

void Game::UpdateTutorial(void)
{
	//条件キーを持っている敵がいなくなったら
	auto& evMng = EventManager::GetInstance();
	if (evMng.IsPlayEvent(EVENT_TYPE::OPEN_TUTORIAL_DOOR))
	{
		//ドアを開ける
		stage_->OpenDoor();

		//カメラの調整
		Camera& camera = SceneManager::GetInstance().GetCamera();
		camera.ChangeMode(Camera::MODE::AUTO_MOVE);
		SetCameraStayTimeAtAutoMove(120.0f);
		camera.SetGoalPos(stage_->GetGoalPosAtDoorOpen());
		camera.SetFocusPos(stage_->GetDoorPos());

		//ステージ１に移行
		updateProgress_ = &Game::UpdateStage1;
		progress_ = GAME_PROGRESS::STAGE_1;

		//敵生成
		enemy_->CreateStageEnemy(stageEnemyData_.allStageInfo[PROGRESS[static_cast<int>(progress_)]]);
	}
}

void Game::UpdateStage1(void)
{
	//条件キーを持っている敵がいなくなったら
	auto& evMng = EventManager::GetInstance();
	if (evMng.IsPlayEvent(EVENT_TYPE::GAME_CLEAR))
	{
		//ゲームクリア
		SceneManager::GetInstance().ChangeScene(std::make_shared<GameClear>());
	}
}

void Game::Draw(void)
{
	DrawString(10, 10, L"GameScene", 0xffffff);

	stage_->Draw();
	ChunkManager::GetInstance().DebugDraw();
	enemy_->Draw();
	player_->Draw();
	ComboManager::GetInstance().Draw();

	//DrawEdge();

	//ポストエフェクトをかけるとき
	if (isDrawPostEffect_) {
		//描画
		(this->*drawPostEffect_)();
	}
	//DrawFormatString(400, 0, 0xffffff, L"%d", GetDrawCallCount());
}

void Game::Release(void)
{
	player_->Release();
	enemy_->Release();
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.Stop(SoundManager::SOUND_NAME::GAME_BGM);	//今まで流していたものを停止

	SingletonRegistry::GetInstance().Delete(SingletonRegistry::DESTROY_TIMING::GAME_END);	//シングルトンの削除
	CollisionManager::GetInstance().DeleteAllCollider();
}

void Game::Reset(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	sndM.AdjustVolume(SoundManager::TYPE::BGM, BGM_VOL);	//前シーンに戻るのでBGMの音量を復活
	//とりあえずメニューからの復帰時は追従に
	//メニュー開く直前に変える可能性大
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);
}

void Game::StartBossFaze(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	ChangeActionDirec(ACTION_DIRECTION::SCAN_LINE);

	//sndM.Stop("NomalBgm");	//今まで流していたものを停止
	//sndM.Stop("BattleBgm");	//今まで流していたものを停止
	//sndM.Play("WarningBgm");//警告音流す

	//演出初期設定
	direcState_ = BOSS_DIRECTION::POST_EFFECT;
	update_ = &Game::DirectionUpdate;
	direcUpdate_ = &Game::DirectionPostEffect;
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FIXED_POINT);	//演出中はカメラ操作を受け付けない
}

void Game::ChangeActionDirec(const ACTION_DIRECTION _direc)
{
	//とりあえずポストエフェクトを描画するように
	isDrawPostEffect_ = true;

	////各ポストエフェクトの描画設定
	//if (_direc == ACTION_DIRECTION::SCAN_LINE) {
	//	drawPostEffect_ = &Game::DrawScanLine;
	//}
	//else if (_direc == ACTION_DIRECTION::BLUR) {
	//	drawPostEffect_ = &Game::DrawBlur;
	//}
	//else if (_direc == ACTION_DIRECTION::JUST_DODGE) {
	//	drawPostEffect_ = &Game::DrawDodgeEffect;
	//}
	//else {
	//	//上記三つ以外の場合はポストエフェクトをかけない
	//	isDrawPostEffect_ = false;
	//}
}

void Game::AttackDataInit(void)
{
	////攻撃の情報入れ
	//atkMng_->AddAttack(PlayerManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD,false, false, PlayerManager::ATTACK_TIME);
	//atkMng_->AddAttack(EnemyManager::ATTACK_NOMAL, AttackManager::ATTACK_TYPE::SWORD, true,false, EnemyManager::ATTACK_TIME, EnemyManager::ATTACK_TIME_START, EnemyManager::ATTACK_TIME_END);
}

void Game::FinishSwitchBgm(void)
{
	SoundManager& sndM = SoundManager::GetInstance();
	//切り換え終了
	switchBgm_ = false;
	//sndM.AdjustVolume(switchBgmStr_, nextBgmVol_);
	////今まで流していたものを停止
	//sndM.Stop(nowBgmStr_);	
	//現在のBGM名と切り替え後のBGM名の切り換え
	auto ret = nowBgmStr_;
	nowBgmStr_ = switchBgmStr_;
	switchBgmStr_ = ret;
	//初期化
	nextBgmVol_ = 0;
}

void Game::StartSlow(void)
{
	if (isSlowEffect_)return;

	auto& scM = SceneManager::GetInstance();
	//スロー演出準備
	slowCnt_ = 0;
	
	//更新スピードを50％に設定
	scM.SetUpdateSpeedRate(SLOW_SPEED_PERCENT);
	//敵もそれに対応
	enemy_->SetAnimSpeedPercent(scM.GetUpdateSpeedRatePercent());

	//プレイヤーのアニメーションも調整
	player_->SetAnimSpeedPercent(scM.GetUpdateSpeedRatePercent());
	player_->SetIsSpecialRedy(true);

	isSlowEffect_ = true;
}

void Game::EndSlow(void)
{
	if (!isSlowEffect_)return;

	auto& scM = SceneManager::GetInstance();
	isSlowEffect_ = false;
	//ChangeActionDirec(ACTION_DIRECTION::NOMAL);
	//更新処理を100％にもどす
	scM.SetUpdateSpeedRate(NORMAL_SPEED_PERCENT);
	enemy_->SetAnimSpeedPercent(scM.GetUpdateSpeedRatePercent());
	player_->SetAnimSpeedPercent(scM.GetUpdateSpeedRatePercent());
	player_->SetIsSpecialRedy(false);
}


void Game::DrawDebug(void)
{
	//SceneManager::GetInstance().GetCamera().DrawDebug();F
	//if (isSlowEffect_) {
	//	DrawString(0, 140, "NOW_SLOW", 0xffffff);
	//}
	//enemy_->DrawDebug();
	//atkMng_->DrawDebug();
	//stage_->DrawDebug();
	//player_->DrawDebug();
}
