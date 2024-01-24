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
	worldTransform_.Initialize();

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;

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

	worldTransformHammer_.Initialize();
	worldTransformHammer_.parent_ = &worldTransform_;
	worldTransformHammer_.translation_.y = 1.2619f;


	//浮遊ギミックの初期化
	InitializeFloatingGimmick();

	
}

// 更新
void Player::Update() { 

	//行動リクエストがあったら
	if (behaviorRequest_) {
		//行動を変更する
		behavior_ = behaviorRequest_.value();
		//各行動ごとの初期化を実行
		switch (behavior_) {
			//通常行動
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
			//攻撃行動
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kJump:
			BehaviorJumpInitialize();
			break;
		}
		//行動リクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
		//通常行動
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
		//攻撃行動
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
		//ジャンプ行動
	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	}

	//浮遊ギミックの更新
	UpdateFloatingGimmick();

	

	//行列を更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();

	
	//変換行列を更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_,
	    worldTransform_.translation_);
	//変換行列を定数バッファに転送
	worldTransform_.TransferMatrix();


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
	//近接武器を描画
	DrawMeleeWeapon(viewProjection);
}

Vector3 Player::GetCenterPosition() const {
	//ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	//ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Player::OnCollision() {
	//ジャンプリクエスト
	behaviorRequest_ = Behavior::kJump;
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

}

void Player::UpdateRootGimmick() {

	worldTransformL_arm_.rotation_.x =
	    std::sin(floatingParameter_) * idleArmAngleMax_ / 180.0f * PI;
	worldTransformR_arm_.rotation_.x =
	    std::sin(floatingParameter_) * idleArmAngleMax_ / 180.0f * PI;
}

void Player::UpdateAttackGimmick() {

	//予備動作の角度
	const float anticipationAngle = -210 / 180.0f * PI;
	// 予備動作の1フレーム増分
	const float anticipationAngleStep = anticipationAngle / anticipationTime_;
	//攻撃動作の時間
	const uint32_t attackTime = 10;
	//攻撃動作の角度
	const float attackAngle = 120 / 180.0f * PI;
	//攻撃動作の１フレーム増分
	const float attackAngleStep = attackAngle / attackTime;

	if (attackParameter_ < anticipationTime_) {
		worldTransformL_arm_.rotation_.x += anticipationAngleStep;
		worldTransformR_arm_.rotation_.x += anticipationAngleStep;
		//worldTransformHammer_.rotation_.x += anticipationAngleStep;
	} else if (attackParameter_ < anticipationTime_ + chargeTime_) {
		worldTransformL_arm_.rotation_.x = anticipationAngle;
		worldTransformR_arm_.rotation_.x = anticipationAngle;
		//worldTransformHammer_.rotation_.x += anticipationAngle;
	} else if (attackParameter_ < anticipationTime_ + chargeTime_ + attackTime) {
		worldTransformL_arm_.rotation_.x += attackAngleStep;
		worldTransformR_arm_.rotation_.x += attackAngleStep;
		//worldTransformHammer_.rotation_.x += attackAngleStep;
	}

	worldTransformHammer_.rotation_.x = worldTransformL_arm_.rotation_.x + PI;
}

void Player::BehaviorRootInitialize() { 
	floatingParameter_ = 0.0f; 
	worldTransformL_arm_.rotation_.x = 0;
	worldTransformR_arm_.rotation_.y = 0;
}

void Player::BehaviorRootUpdate() {

	//ギミック更新
	UpdateFloatingGimmick();
	UpdateRootGimmick();

	const float speed = 0.3f;

	velocity_ = {};

	// 移動量
	/*Vector3 move = {
	    0.0f,
	    0.0f,
	    0.0f,
	};*/

	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		    velocity_.x = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		    velocity_.x = -1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		    velocity_.z = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		    velocity_.z = -1.0f;
	}

	velocity_ = Normalize(velocity_) * speed;
	
	if (viewProjection_) {
		    // カメラの回転行列
		    Matrix4x4 matRotX = MakeRotateXMatrix(viewProjection_->rotation_.x);
		    Matrix4x4 matRotY = MakeRotateYMatrix(viewProjection_->rotation_.y);
		    Matrix4x4 matRotZ = MakeRotateZMatrix(viewProjection_->rotation_.z);
		    // 回転行列の合成
		    Matrix4x4 matRot = matRotZ * matRotX * matRotY;
		    // 移動量をカメラの回転に合わせて回転させる
		    velocity_ = TransformNormal(velocity_, matRot);
	}
	// 移動
	worldTransform_.translation_ += velocity_;

	// 移動ベクトルのY軸周り角度
	worldTransform_.rotation_.y = std::atan2(velocity_.x,velocity_.z);

	if (Input::GetInstance()->PushKey(DIK_A)) {
		   
		// 攻撃リクエスト
		    behaviorRequest_ = Behavior::kAttack;
	}
	//ジャンプボタンを押したら
	else if (Input::GetInstance()->PushKey(DIK_S)) {

		//ジャンプリクエスト
		    behaviorRequest_ = Behavior::kJump;
	}
}

void Player::BehaviorAttackInitialize() { 
	attackParameter_ = 0;
	worldTransformL_arm_.rotation_.x = 0;
	worldTransformR_arm_.rotation_.x = 0;
}

void Player::BehaviorAttackUpdate() {

	//攻撃行動の時間
	const uint32_t behaviorAttackTime = 60;

	//ギミックの更新
	UpdateAttackGimmick();

	//攻撃移動
	if (anticipationTime_<=attackParameter_&&
	    attackParameter_ < anticipationTime_ + chargeTime_) {
		    const float speed = 0.5f;
			//移動量
		    Vector3 move = {0.0f, 0.0f, speed};

			//回転行列
		    Matrix4x4 matRotX = MakeRotateXMatrix(worldTransform_.rotation_.x);
		    Matrix4x4 matRotY = MakeRotateYMatrix(worldTransform_.rotation_.y);
		    Matrix4x4 matRotZ = MakeRotateZMatrix(worldTransform_.rotation_.z);
			//回転行列の合成
		    Matrix4x4 matRot = matRotZ * matRotX * matRotY;

			//移動量を自キャラの向きに合わせて回転させる
		    move = TransformNormal(move, matRot);
			//移動
		    worldTransform_.translation_ += move;
	}

	//規定の時間経過で通常行動に戻る
	if (++attackParameter_ >= behaviorAttackTime) {
		    behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorJumpInitialize() {

	worldTransformBody_.translation_.y = 0;
	worldTransformL_arm_.rotation_.x = 0;
	worldTransformR_arm_.rotation_.x = 0;

	//ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	//ジャンプ初速を与える
	velocity_.y = kJumpFirstSpeed;
}

void Player::BehaviorJumpUpdate() {
	//移動
	worldTransform_.translation_ += velocity_;
	//重力加速度
	const float kGravityAcceleration = 0.05f;
	//加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};
	//加速する
	velocity_ += accelerationVector;

	//着地
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0;
		//ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::DrawMeleeWeapon(const ViewProjection& viewProjection) { 
	switch (behavior_) {
		//通常行動
	case Behavior::kRoot:
	default:
		break;
		//攻撃行動
	case Behavior::kAttack:
		//ハンマーを描画
		models_[kModelIndexHammer]->Draw(worldTransformHammer_, viewProjection);
		break;
	}

}
