﻿#pragma once

#include"Model.h"
#include"WorldTransform.h"

//スカイドーム
//<summary>
class Skydome {
public:
	//初期化
	void Initialize(Model*model);
	
	//更新
	void Update();

	//描画
	void Drow(const ViewProjection& viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
