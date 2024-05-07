#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class Player {
public:

	void Update();

	void Draw(ViewProjection& viewprojection);

	void Initialize(Model* model, uint32_t textureHandle);


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;

};

