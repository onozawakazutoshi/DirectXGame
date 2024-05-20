#include "Player.h"
#include "TextureManager.h"
#include "Input.h"
#include<math.h>
#include "ImGuiManager.h"
#include "WorldTransform.h"

void Player::Update() {
	worldTransform_.TransferMatrix(); 
	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;
	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.y += kRotSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.y -= kRotSpeed;
	}

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	float movef[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	Attack();

	if (bullet_) {
		bullet_->Update();
	}
	for (PlayerBullet* bullet : bullets) {
		bullet->Update();
	}
	
	
	//ImGui::Begin("Debug");
	//ImGui::InputFloat3("", movef);
	//ImGui::SliderFloat3("", movef, -50.0f, 50.0f);
	//ImGui::End();
	worldTransform_.translation_.x = movef[0];
	worldTransform_.translation_.y = movef[1];
	worldTransform_.translation_.z = movef[2];

	const float kMoveLimitx = 50;
	const float kMoveLimity = 50;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x,-kMoveLimitx);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitx);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimity);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimity);

	worldTransform_.matWorld_= WorldTransform::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}


void Player::Draw(ViewProjection& viewprojection) { 
	model_->Draw(worldTransform_, viewprojection, textureHandle_);
	if (bullet_) {
		bullet_->Draw(viewprojection);
	}
	for (PlayerBullet* bulet_ : bullets) {
	
	bulet_->Draw(viewprojection);
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	textureHandle_ = textureHandle; 
	model_ = model;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Attack() { 
	if (input_->PushKey(DIK_T)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = WorldTransform::Transform(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		bullet_ = newBullet;
		bullets.push_back(newBullet);
	}
	/*if (bullet_) {
		delete bullet_;
		bullet_ = nullptr;
	}*/
	
}

Player::~Player() {
	for (PlayerBullet* bulet_ : bullets) {
		delete bulet_;
	}
}
