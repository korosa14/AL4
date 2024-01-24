#pragma once

#include"Vector3.h"
#include<bitset>

//衝突検出オブジェクト
class Collider {
public:
	//デフォルト衝突判定
	const float kDefaultRadius = 1.5f;

	//衝突時に呼ばれる関数
	virtual void OnCollision() {}
	//中心座標を収得
	virtual Vector3 GetCenterPosition() const = 0;
	//半径を収得
	float GetRadius() const { return radius_; }
	//半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	
private:
	//衝突半径
	float radius_ = kDefaultRadius;
};

