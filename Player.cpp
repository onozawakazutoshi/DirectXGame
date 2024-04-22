#include "Player.h"
#include "TextureManager.h"



void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw(ViewProjection& viewprojection) { model_->Draw(worldTransform_, viewprojection, textureHandle_); }

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	textureHandle_ = textureHandle; 
	model_ = model;

	worldTransform_.Initialize();
}
