#include "Player.h"
#include "TextureManager.h"
#include "Input.h"
#include<math.h>
#include "ImGuiManager.h"


Matrix4x4 MakeAffinMatrix(Vector3& S, Vector3& R, Vector3& T);
Vector3 Transform(Vector3 vector, Matrix4x4 matrix);
Matrix4x4 MAkeTranslateMatrix(Vector3& vector3);
Matrix4x4 MAkeScaleMatrix(Vector3& vector3);
Matrix4x4 MakeRotateXMatrix(Vector3& vector);
Matrix4x4 MakeRotateYMatrix(Vector3& vector);
Matrix4x4 MakeRotateZMatrix(Vector3& vector);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

void Player::Update() {
	worldTransform_.TransferMatrix(); 
	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	float movef[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};
	
	
	ImGui::Begin("Debug");
	//ImGui::InputFloat3("", movef);
	ImGui::SliderFloat3("", movef, -50.0f, 50.0f);
	ImGui::End();
	worldTransform_.translation_.x = movef[0];
	worldTransform_.translation_.y = movef[1];
	worldTransform_.translation_.z = movef[2];

	const float kMoveLimitx = 50;
	const float kMoveLimity = 50;
	worldTransform_.translation_.x = max(worldTransform_.translation_.x,-kMoveLimitx);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitx);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimity);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimity);

	worldTransform_.matWorld_=MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}
Matrix4x4 MakeRotateXMatrix(Vector3& vector) {
	Matrix4x4 matrix{0};
	matrix.m[1][1] = std::cosf(vector.x);
	matrix.m[1][2] = std::sinf(vector.x);
	matrix.m[2][1] = -std::sinf(vector.x);
	matrix.m[2][2] = std::cosf(vector.x);
	matrix.m[0][0] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

Matrix4x4 MakeRotateYMatrix(Vector3& vector) {
	Matrix4x4 matrix{0};
	matrix.m[0][0] = std::cosf(vector.y);
	matrix.m[0][2] = -std::sinf(vector.y);
	matrix.m[2][0] = std::sinf(vector.y);
	matrix.m[2][2] = std::cosf(vector.y);
	matrix.m[1][1] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

Matrix4x4 MakeRotateZMatrix(Vector3& vector) {
	Matrix4x4 matrix{0};
	matrix.m[0][0] = std::cosf(vector.z);
	matrix.m[0][1] = std::sinf(vector.z);
	matrix.m[1][0] = -std::sinf(vector.z);
	matrix.m[1][1] = std::cosf(vector.z);
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 ans = {0};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ans.m[i][j] += m1.m[i][0] * m2.m[0][j];
			ans.m[i][j] += m1.m[i][1] * m2.m[1][j];
			ans.m[i][j] += m1.m[i][2] * m2.m[2][j];
			ans.m[i][j] += m1.m[i][3] * m2.m[3][j];
		}
	}
	return ans;
}
Matrix4x4 MAkeTranslateMatrix(Vector3& vector3) {
	Matrix4x4 ans{0};
	ans.m[0][0] = vector3.x;
	ans.m[1][1] = vector3.y;
	ans.m[2][2] = vector3.z;
	ans.m[3][3] = 1;
	return ans;
}
Matrix4x4 MAkeScaleMatrix(Vector3& vector3) {
	Matrix4x4 ans{0};
	ans.m[0][0] = 1;
	ans.m[1][1] = 1;
	ans.m[2][2] = 1;
	ans.m[3][0] = vector3.x;
	ans.m[3][1] = vector3.y;
	ans.m[3][2] = vector3.z;
	ans.m[3][3] = 1;
	return ans;
}
Vector3 Transform(Vector3 vector, Matrix4x4 matrix) {
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}
Matrix4x4 MakeAffinMatrix(Vector3& S, Vector3& R, Vector3& T) {
	Matrix4x4 ans{0};
	Matrix4x4 matrixS = MAkeTranslateMatrix(S);
	Matrix4x4 matrixR = Multiply(MakeRotateXMatrix(R), Multiply(MakeRotateYMatrix(R), MakeRotateZMatrix(R)));
	Matrix4x4 matrixT = MAkeScaleMatrix(T);
	ans = Multiply(matrixS, Multiply(matrixR, matrixT));
	return ans;
}


void Player::Draw(ViewProjection& viewprojection) { model_->Draw(worldTransform_, viewprojection, textureHandle_); }

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	textureHandle_ = textureHandle; 
	model_ = model;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}
