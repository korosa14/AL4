#pragma once
#include "BaseCharacter.h"

//敵
class Enemy : public BaseCharacter{
public:
	enum  {
		kModelIndexBody,
		kModelIndexL_arm,
		kModelIndexR_arm,
	};

	//浮遊ギミックの媒介変数
	float floatingParameter = 0.0f;

	//初期化
	void Initialize(const std::vector<Model*>& models) override;

	//更新
	void Update() override;

	//描画
	void Draw(const ViewProjection& viewProjection) override;

	//ギミック更新
	void UpdateGimmick();

protected:
	//ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

};
