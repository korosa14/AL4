#pragma once

#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"Input.h"

class Player {
public:
	//初期化
	void Initialize(Model*modelBody,Model*modelHead,Model*modelL_arm,Model*modelR_arm);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	//ワールド変換データを取得
	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	//浮遊移動のサイクル<sec>
	int32_t floationgCycle_ = 90;
	//浮遊振動
	float floatingAmplitude_ = 0.2f;
	//待機モーションの腕角度最大値
	float idleArmAngleMax_ = 30.0f;
	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	//浮遊ギミックサイクル
	int32_t floatingCycle_ = 90;
	//モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	//インプット
	Input* input_ = nullptr;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	
	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	//浮遊ギミック更新
	void UpdateFloatingGimmick();
};
