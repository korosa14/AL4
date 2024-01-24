#include "BaseCharacter.h"


void BaseCharacter::Initialize(const std::vector<Model*>& models) { 
	models_ = models;
	worldTransform_.Initialize();
}

void BaseCharacter::Update() { worldTransform_.UpdateMatrix(); }

void BaseCharacter::Draw(const ViewProjection& viewProjection) { 
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}


void BaseCharacter::SetLocalPosition(const Vector3& position) {
	worldTransform_.translation_ = position;
}

void BaseCharacter::SetLocalRotation(const Vector3& rotation) {
	worldTransform_.rotation_ = rotation;
}

Vector3 BaseCharacter::GetCenterPosition() const { 
	
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を収得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
