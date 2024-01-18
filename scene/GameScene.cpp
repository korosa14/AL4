#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"MathUtilityForText.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	//delete spriteBG_;//BG
}

void GameScene::Initialize() {

	// クラスの生産&初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.translation_ = {0.0f, 2.0f, -10.0f};
	viewProjection_.Initialize();

	// デバックカメラ
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(2000.0f);

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示を参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	
	//3Dモデルの生産
	modelCube_.reset(Model::Create());
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelHammer_.reset(Model::CreateFromOBJ("hammer"));
	modelEnemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	modelEnemyL_arm_.reset(Model::CreateFromOBJ("needle_L_arm", true));
	modelEnemyR_arm_.reset(Model::CreateFromOBJ("needle_R_arm", true));

	
	
	//地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());
	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());
	
	//自キャラ
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(), modelHammer_.get()};
	// 自機
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(playerModels);
	player_->SetViewProjection(&followCamera_->GetViewProJection());
	//自キャラのワールドトランスフォーマーを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//敵モデル
	std::vector<Model*> enemyModels = {
	    modelEnemyBody_.get(), modelEnemyL_arm_.get(), modelEnemyR_arm_.get()
	};
	//敵の生産
	enemy_ = std::make_unique<Enemy>();
	//敵の初期化
	enemy_->Initialize(enemyModels);
	enemy_->SetLocalPosition(Vector3(10, 0, 20));
	enemy_->SetLocalRotation(Vector3(0, PI, 0));
}

void GameScene::Update() { 
	/*skydome_->Update();
	player_->Update();
	ground_->Update();*/

	//追従カメラの更新
	followCamera_->Update();

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
		viewProjection_.matView = followCamera_->GetViewProJection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProJection().matProjection;
	}
	//ビュウープロジェクションの転送
	viewProjection_.TransferMatrix();

	//天球
	skydome_->Update();
	//地面
	ground_->Update();
	//自キャラ
	player_->Update();
	//敵
	enemy_->Update();
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
	enemy_->Draw(viewProjection_);
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
