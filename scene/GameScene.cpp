#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	//delete spriteBG_;//BG
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ビュウープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.translation_ = {0.0f, 2.0f, -10.0f};
	viewProjection_.Initialize();
	//BG(2Dスプライト)
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生産
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	modelFighter_.reset(Model::CreateFromOBJ("float", true));
	//クラスの生産
	player_ = std::make_unique<Player>();
	ground_ = std::make_unique<Ground>();
	skydome_ = std::make_unique<Skydome>();
	//クラスの初期化
	player_->Initialize(modelFighter_.get());
	ground_->Initialize(modelGround_.get());
	skydome_->Initialize(modelSkydome_.get());

	//デバックカメラ
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(2000.0f);

	//追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	player_->SetViewProjection(&followCamera_->GetViewProJection());
	//自キャラのワールドトランスフォーマーを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示を参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	
}

void GameScene::Update() { 
	/*skydome_->Update();
	player_->Update();
	ground_->Update();*/

	//デバックカメラの更新
	if (input_->TriggerKey(DIK_0)) {
		//フラグをトグル
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	} else {
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProJection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProJection().matProjection;
	}
	//ビュウープロジェクションの転送
	viewProjection_.TransferMatrix();

	player_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	//背景
	/*spriteBG_->Draw();*/

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	skydome_->Drow(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
