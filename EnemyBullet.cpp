#include "EnemyBullet.h"
#include "TextureManager.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransform_.Initialize();
	
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	//playerpos_ = player_.GetWorldPosition();
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos = WorldTransform::Transform({1, 1, 1}, worldTransform_.matWorld_);
	return worldPos;
}
