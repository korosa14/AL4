#include "Player.h"
#include"ImGuiManager.h"
#include<cassert>
#include"Input.h"
#include"MathUtilityForText.h"
#include"Vector3.h"

//初期化
void Player::Initialize(const std::vector<Model*>& models) {
	//基底クラスの初期化
	BaseCharacter::Initialize(models);
	
	//ワールド変換の初期化
	worldTransformBase_.Initialize();

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransformBase_;

	worldTransformHead_.Initialize();
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformHead_.translation_.y = 1.5038f;

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -0.5275f;
	worldTransformL_arm_.translation_.y = 1.2619f;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = +0.5275f;
	worldTransformR_arm_.translation_.y = 1.2619f;

	//浮遊ギミックの初期化
	InitializeFloatingGimmick();

	//インプット
	//input_ = Input::GetInstance();
}

// 更新
void Player::Update() { 


	//浮遊ギミックの更新
	UpdateFloatingGimmick();

		const float speed = 0.3f;
		
		//移動量
		Vector3 move = {
		   0.0f,0.0f,0.0f,
		};
	
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
			// カメラの回転行列
			Matrix4x4 matRotX = MakeRotateXMatrix(viewProjection_->rotation_.x);
			Matrix4x4 matRotY = MakeRotateYMatrix(viewProjection_->rotation_.y);
			Matrix4x4 matRotZ = MakeRotateZMatrix(viewProjection_->rotation_.z);
			// 回転行列の合成
			Matrix4x4 matRot = matRotZ * matRotX * matRotY;
			// 移動量をカメラの回転に合わせて回転させる
			move = TransformNormal(move, matRot);
		}
		//移動
		worldTransformBase_.translation_ += move;

		// 移動ベクトルのY軸周り角度
		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);

	

	//行列を更新
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	
	//変換行列を更新
	worldTransformBase_.matWorld_ = MakeAffineMatrix(
	    worldTransformBase_.scale_, worldTransformBase_.rotation_,
	    worldTransformBase_.translation_);
	//変換行列を定数バッファに転送
	worldTransformBase_.TransferMatrix();


	ImGui::Begin("Player");
	ImGui::SliderFloat3(
	    "Head Translation", reinterpret_cast<float*>(&worldTransformHead_.translation_), -10.0f,
	    -10.0f);
	ImGui::SliderFloat3(
	    "Head Translation", reinterpret_cast<float*>(&worldTransformL_arm_.translation_), -10.0f,
	    10.0f);
	ImGui::SliderFloat3(
	    "Head Translation", reinterpret_cast<float*>(&worldTransformR_arm_.translation_), -10.0f,
	    10.0f);
	ImGui::SliderInt("floatintgCycie", reinterpret_cast<int*>(&floationgCycle_), 1,200);
	ImGui::SliderFloat("floatingAmplitude", &floatingAmplitude_, 0.0f, 10.0f);
	ImGui::SliderFloat("idleArmAngleMax_", &idleArmAngleMax_, 0.0f, 180.0f);
	ImGui::End();

}

//描画
void Player::Draw(const ViewProjection& viewProjection) {
	//3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() { 
	
	const float floatingStep = PI * 2.0f / floatingCycle_;

	//浮遊ギミックを更新
	floatingParameter_ += floatingStep;
	//2πを超えたら０に戻す
	floatingParameter_ = fmodf(floatingParameter_,PI * 2.0f);
	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;
	worldTransformL_arm_.rotation_.x =
	    std::sin(floatingParameter_) * idleArmAngleMax_ / 180.0f * PI;
	worldTransformR_arm_.rotation_.x =
	    std::sin(floatingParameter_) * idleArmAngleMax_ / 180.0f * PI;

}
