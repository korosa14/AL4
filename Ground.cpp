#include"Ground.h"
#include<cassert>

//初期化
void Ground::Initialize(Model* model) {

	// NULLポインタチェック
	assert(model);
	//メンバ変数に受け取った値を代入
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();
};

void Ground::Update(){ 
	worldTransform_.TransferMatrix(); 
};

void Ground::Draw(const ViewProjection& viewProjection){
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}