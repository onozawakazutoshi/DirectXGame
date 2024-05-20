#include "PlayerBullet.h"
#include "TextureManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;


}

void PlayerBullet::Update() { 
	
	worldTransform_.UpdateMatrix();
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }
