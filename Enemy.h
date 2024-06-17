#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "Player.h"

class Player;
class GameScene;
class Enemy {
public:
	void Update();

	void Draw(ViewProjection& viewprojection);

	void Initialize(Model* model, uint32_t textureHandle,Vector3 vec);

	void Attack();

	~Enemy();

	void AttackInitialize();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	enum class Phase {
		Approach,
		Leave
	};

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetworldPosition();

	void OnCollision();	

	const std::list<EnemyBullet*>& GetBullets() const { return bullets; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	Phase phase_ = Phase::Approach;

	
	std::list<EnemyBullet*> bullets;
	static const int kFireInterval = 60;
	int32_t time = 0;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;
};
