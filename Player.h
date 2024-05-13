#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"

class Player {
public:
	void Update();

	void Draw(ViewProjection& viewprojection);

	void Initialize(Model* model, uint32_t textureHandle);

	void Attack();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	
	PlayerBullet* bullet_ = nullptr;
};

