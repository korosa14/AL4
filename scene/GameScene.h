#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Player.h"
#include"Ground.h"
#include"Skydome.h"
#include"DebugCamera.h"
#include"FollowCamera.h"

#include<memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	////BG(スプライト)
	//uint32_t textureHandleBG_ = 0;
	//Sprite* spriteBG_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//ビュウープロジェクション
	ViewProjection viewProjection_;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//モデルデータ
	std::unique_ptr<Model> modelCube_;
	//std::unique_ptr<Model> modelFighter_;

	std::unique_ptr<Model> modelSkydome_;
	std::unique_ptr<Model> modelGround_;
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	// 自キャラ
	std::unique_ptr<Player> player_;
	//地面
	std::unique_ptr<Ground> ground_;
	//天球
	std::unique_ptr<Skydome> skydome_;
	//デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	//デバックカメラ有効
	bool isDebugCameraActive_ = false;
	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

};
