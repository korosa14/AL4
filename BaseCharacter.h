#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class BaseCharacter {
public:

	//初期化
	virtual void Initialize(const std::vector<Model*>& models);

	// 更新
	virtual void Update();

	//描画
	virtual void Draw(const ViewProjection& viewProjection);

	//ローカル座標を設定
	//親がない場合はワールド座標となる
	 void SetLocalPosition(const Vector3& position);

	//ローカル角度を設定
	 void SetLocalRotation(const Vector3& rotation);

	 //ローカル座標を取得

	 //親がない場合はワールド座標となる
	 const Vector3& GetLocalPosition() const { return worldTransform_.translation_; };

	 //ワールド変換データを取得
	 //ワールド変換データ
	 const WorldTransform& GetWorldTransform() { return worldTransform_; }

protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;
};
