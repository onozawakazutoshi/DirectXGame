#include "GameScene.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include <cassert>
#include <fstream>
//#include <string>



GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_; 
	delete model_;
	delete debugCamera_;
	delete enemy_;
	delete modelskydome_;
	delete railcamera_;
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
	voiceHandle_ = audio_->PlayWave(soundDataHandle_,true );
	PrimitiveDrawer::GetInstance()->SetViewProjection(&ViewProjection_);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	player_ = new Player;
	player_->Initialize(model_, textureHandle_);
	enemy_ = new Enemy;
	enemy_->Initialize(model_, textureHandle_,Vector3(0,0,0));
	enemy_->SetPlayer(player_);
	modelskydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new skydome;
	skydome_->Initalize(modelskydome_, textureHandle_);
	railcamera_ = new RailCamera;
	railcamera_->Initialize(ViewProjection_.rotation_);

	enemy_->SetGameScene(this);
	//enemybullets_->Initialize()
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
	enemy_->Update();

	if (isDebugCameraaActive_) {
		debugCamera_->Update();
		ViewProjection_.matView = debugCamera_->GetViewProjection().matView;
		ViewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		ViewProjection_.TransferMatrix();
	} else {
		railcamera_->Update();
		ViewProjection_.matView = railcamera_->GetViewProjection().matView;
		ViewProjection_.matProjection = railcamera_->GetViewProjection().matProjection;
		ViewProjection_.TransferMatrix();
	}
	CheckAllCollisions();
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
	enemy_->Draw(ViewProjection_);
	skydome_->Draw(ViewProjection_);
	

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

void GameScene::LoadEnemyPopData() { 
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();

}

void GameScene::UpdateEnemyPopCommands() {
	std::string line;
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);
		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			enemy_->Initialize(model_, textureHandle_, Vector3(x, y, z));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());
			stopflag = true;
			stoptime = waitTime;

			break;
			
		}
	}
	if (stopflag) {
		stoptime--;
		if (stoptime <= 0) {
			stopflag = false;
		}
		return;
	}
}

void GameScene::CheckAllCollisions() { 
	
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		float length = sqrtf(
		    ((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y))  +
		    (posA.z - posB.z) * (posA.z - posB.z));

		if (length < 1) {
			player_->OnCollision();

			bullet->OnCollision();
		}
	}
	#pragma endregion

	

	#pragma region
	posA = enemy_->GetworldPosition();

	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();

		float length = sqrtf(((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y)) + (posA.z - posB.z) * (posA.z - posB.z));

		if (length < 1) {
			enemy_->OnCollision();

			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region
	posA = player_->GetWorldPosition();
	posB = enemy_->GetworldPosition();

	float length = sqrtf(((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y)) + (posA.z - posB.z) * (posA.z - posB.z));

	if (length < 1) {
		player_->OnCollision();

		enemy_->OnCollision();
	}
	
	#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { 
	enemybullets_.push_back(enemyBullet); 
}
