#pragma once

#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"Input.h"

class Player {
public:
	//初期化
	void Initialize(Model*model);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	//ワールド変換データを取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//インプット
	Input* input_ = nullptr;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	
};
