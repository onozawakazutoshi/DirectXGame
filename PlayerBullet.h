#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class PlayerBullet{

	public:

		void Initialize(Model* model, const Vector3& position,const Vector3& velocity);


		void Update();

		void Draw(const ViewProjection& viewProjection);

	private:
	    Model* model_ = new Model;
	    uint32_t textureHandle_ = 0u;
	    WorldTransform worldTransform_;
	    Vector3 velocity_;
};
