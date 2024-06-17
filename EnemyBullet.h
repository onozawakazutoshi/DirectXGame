#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Player.h"

class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();


private:
	Model* model_ = new Model;
	uint32_t textureHandle_ = 0u;
	WorldTransform worldTransform_;
	Vector3 velocity_;
	Vector3 playerpos_;
	Player player_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	
	bool isDead_ = false;
};
