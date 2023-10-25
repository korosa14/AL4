#include "Skydome.h"
#include<cassert>

void Skydome::Initialize(Model* model) {

	// NULLポインタチェック
	assert(model);
	// メンバ変数に受け取った値を代入
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update() { 
	worldTransform_.TransferMatrix(); 
}

void Skydome::Drow(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}
