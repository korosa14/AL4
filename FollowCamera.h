#pragma once

#include "ViewProjection.h"
#include"WorldTransform.h"

//追従カメラ
class FollowCamera {
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProJection() { return viewProjection_; }

private:
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//追従対象
	const WorldTransform* target_ = nullptr;

};
