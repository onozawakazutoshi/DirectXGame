#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//スプライト
	Sprite* sprite_ = nullptr;

	WorldTransform WorldTransform_;

	ViewProjection ViewProjection_;

private: // メンバ変数 
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Model* model_ = nullptr;
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	float inputFloat3[3] = {0, 0, 0};
	DebugCamera* debugCamera_ = nullptr;

	Player* player_ = nullptr;
	bool isDebugCameraaActive_ = false;
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
