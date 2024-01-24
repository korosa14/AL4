#pragma once

#include"Collider.h"
#include<list>
#include<memory>

//衝突マネージャー
class CollisionManager {

public:
	//リセット
	void Reset();
	//全ての当たり判定チェック
	void CheckAllCollisions();
	//コライダーの登録
	void AddCollider(Collider* collider);

private:
	//コライダー
	std::list<Collider*> colliders_;
	//コライダー２つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};
