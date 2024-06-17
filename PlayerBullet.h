#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class PlayerBullet{

	public:

		void Initialize(Model* model, const Vector3& position,const Vector3& velocity);


		void Update();

		void Draw(const ViewProjection& viewProjection);

		void OnCollision();

		bool IsDead() const { return isDead_; }
		
		Vector3 GetWorldPosition();

	private:
	    Model* model_ = new Model;
	    uint32_t textureHandle_ = 0u;
	    WorldTransform worldTransform_;
	    Vector3 velocity_;

		static const int32_t kLifeTime = 60 * 5;
	    int32_t deathTimer_ = kLifeTime;

	    bool isDead_ = false;
};
