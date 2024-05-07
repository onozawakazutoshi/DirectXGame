#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include <cassert>


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_; 
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("cube/cube.jpg");
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	WorldTransform_.Initialize();
	ViewProjection_.Initialize();
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&ViewProjection_);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	player_ = new Player;
	player_->Initialize(model_, textureHandle_);

	
}

void GameScene::Update() { 
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);
	if (input_->TriggerKey(DIK_SPACE)) {
	
		audio_->StopWave(voiceHandle_);
	}

	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraaActive_ = true;
	} else if (input_->TriggerKey(DIK_1)) {
		isDebugCameraaActive_ = false;
	}

	ImGui::Begin("Debug1");
	ImGui::Text("kamata Tarou %d %d %d", 200, 12, 31);
	ImGui::End();

	ImGui::Begin("Debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);

	ImGui::SliderFloat3("slideFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

	ImGui::ShowDemoWindow();

#endif
	player_->Update();
	if (isDebugCameraaActive_) {
		debugCamera_->Update();
		ViewProjection_.matView = debugCamera_->GetViewProjection().matView;
		ViewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		ViewProjection_.TransferMatrix();
	} else {
		ViewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	player_->Draw(ViewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 3dモデル描画
	
	//model_->Draw(WorldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
