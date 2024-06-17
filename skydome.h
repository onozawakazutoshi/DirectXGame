#pragma once
#include "WorldTransform.h"
#include "Model.h"


class skydome {
public:
	void Initalize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewprojection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

};
