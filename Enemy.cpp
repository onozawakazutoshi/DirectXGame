#include "Enemy.h"


void Enemy::Update() {

	bullets.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.TransferMatrix();
	Vector3 move = {0, 0, 0};

	//const float kCharacterSpeed = 0.2f;
	//const float kRotSpeed = 0.02f;

	switch (phase_) {
	case Phase::Approach:
	default:
		worldTransform_.translation_.z -= 0.1f;
		
		time--;
		if (time <= 0) {
			Enemy::Attack();
			time = kFireInterval;
		}


		if (worldTransform_.translation_.z < -20.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		worldTransform_.translation_.x -= 0.1f;
		worldTransform_.translation_.y += 0.1f;
		worldTransform_.translation_.z -= 0.1f;
		if (worldTransform_.translation_.z < -50.0f) {
			worldTransform_.translation_.z = 0;
			worldTransform_.translation_.x = 5;
			worldTransform_.translation_.y = 0;
			phase_ = Phase::Approach;
			
		}
		time--;
		if (time <= 0) {
			Enemy::Attack();
			time = kFireInterval;
		}

		break;


	}

	float movef[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	

	// ImGui::Begin("Debug");
	// ImGui::InputFloat3("", movef);
	// ImGui::SliderFloat3("", movef, -50.0f, 50.0f);
	// ImGui::End();
	worldTransform_.translation_.x = movef[0];
	worldTransform_.translation_.y = movef[1];
	worldTransform_.translation_.z = movef[2];

	const float kMoveLimitx = 50;
	const float kMoveLimity = 50;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitx);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitx);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimity);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimity);


	
	for (EnemyBullet* bullet : bullets) {
		bullet->Update();
	}

	worldTransform_.matWorld_ = WorldTransform::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	

}

void Enemy::Draw(ViewProjection& viewprojection) {
	
	model_->Draw(worldTransform_, viewprojection, textureHandle_); 
	
	for (EnemyBullet* bulet_ : bullets) {

		bulet_->Draw(viewprojection);
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle,Vector3 vec) {
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	Enemy::AttackInitialize();
	worldTransform_.translation_.x = vec.x;
	worldTransform_.translation_.y = vec.y;
	worldTransform_.translation_.z = vec.z;
}

void Enemy::Attack() {
	EnemyBullet* newBullet = new EnemyBullet();

	const float kBulletSpeed = 0.01f;

	Vector3 player = player_->GetWorldPosition(); 
	Vector3 enemy = GetworldPosition();

	Vector3 vec = {enemy.x - player.x, enemy.y - player.y, enemy.z - player.z};
	vec = WorldTransform::Transform(vec, worldTransform_.matWorld_);
	vec = {-vec.x * kBulletSpeed, -vec.y * kBulletSpeed, -vec.z * kBulletSpeed};

	Vector3 velocity(0, 0, kBulletSpeed);
	velocity = WorldTransform::Transform(velocity, worldTransform_.matWorld_);


		newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, vec);
		
		bullets.push_back(newBullet);
}

Enemy::~Enemy() {
	for (EnemyBullet* bulet_ : bullets) {
		delete bulet_;
	}
}

void Enemy::AttackInitialize() { time = kFireInterval; }

Vector3 Enemy::GetworldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	
	
	return worldPos; 
	
}

void Enemy::OnCollision() {}
