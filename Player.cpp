#include "Player.h"
#include"ImGuiManager.h"
//#include"Vector3.h"
#include<cassert>
#include"Input.h"
#include"MathUtilityForText.h"

//初期化
void Player::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);

	//メンバ変数に受け取った値を代入
	//textureHandle_ = textureHandle;
	model_ = model;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();

	//インプット
	input_ = Input::GetInstance();
}
// 更新
void Player::Update() { 

	const float speed = 0.3f;
	Vector3 move = {0.0f, 0.0f, 0.0f};
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		move.x = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		move.x = -1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		move.z = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		move.z = -1.0f;
	}

	move = Normalize(move) * speed;

	if (viewProjection_) {
		//カメラの回転行列
		Matrix4x4 matRotX = MakeRotateXMatrix(viewProjection_->rotation_.x);
		Matrix4x4 matRotY = MakeRotateYMatrix(viewProjection_->rotation_.y);
		Matrix4x4 matRotZ = MakeRotateZMatrix(viewProjection_->rotation_.z);
		//回転行列の合成
		Matrix4x4 matRot = matRotZ * matRotX * matRotY;
		//移動量をカメラの回転に合わせて回転させる
		move = TransformNormal(move, matRot);
	}
	worldTransform_.translation_ += move;
	/*worldTransform_.TransferMatrix();*/

	//移動ベクトルのY軸周り角度
	worldTransform_.rotation_.y = std::atan2(move.x, move.z);

	//変換行列を更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_,
		worldTransform_.translation_);
	//変換行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

//描画
void Player::Draw(const ViewProjection& viewProjection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
	                                           // textureHandle_
}
