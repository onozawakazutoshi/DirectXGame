#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	void Update();

	void Draw(ViewProjection& viewprojection);

	void Initialize(Model* model, uint32_t textureHandle);

	void Attack();

	~Player();

	Vector3 GetWorldPosition();
	
	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	
	//PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets;

	WorldTransform worldTransform3DReticle_;
};

