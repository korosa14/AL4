#pragma once

#include"Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"Input.h"
#include"BaseCharacter.h"
#include<optional>


//自キャラ
class Player :public BaseCharacter {
public:
	enum {
		kModelIndexBody,
		kModelIndexHead, 
		kModelIndexL_arm, 
		kModelIndexR_arm,
		kModelIndexHammer,
	};

	//振る舞い
	enum class Behavior {
		kRoot,//通常状態
		kAttack,//攻撃中
	};

	//初期化
	void Initialize(const std::vector<Model*>&models)override;
	//更新
	void Update()override;
	//描画
	void Draw(const ViewProjection& viewProjection)override;

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

	//予備動作の時間
	const uint32_t anticipationTime_ = 15;
	//溜めの時間
	const uint32_t chargeTime_ = 10;


	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	//浮遊ギミックサイクル
	int32_t floatingCycle_ = 90;
	//攻撃ギミックの媒介変数
	uint32_t attackParameter_ = 0;

	// インプット
	Input* input_ = nullptr;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	
	//行動
	Behavior behavior_ = Behavior::kRoot;
	//行動リクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	
	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	//浮遊ギミック更新
	void UpdateFloatingGimmick();
	//通常ギミック更新
	void UpdateRootGimmick();
	//攻撃ギミック更新
	void UpdateAttackGimmick();

	//通常行動初期化
	void BehaviorRootInitialize();

	//通常行動更新
	void BehaviorRootUpdate();

	//攻撃行動初期化
	void BehaviorAttackInitialize();

	//攻撃行動更新
	void BehaviorAttackUpdate();

	//近接武器描画
	void DrawMeleeWeapon(const ViewProjection& viewProjection);
};
