#include "WorldTransform.h"

#include <cmath>



void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffinMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}

Matrix4x4 WorldTransform::MakeAffinMatrix(Vector3& S, Vector3& R, Vector3& T) { Matrix4x4 ans{0};
	Matrix4x4 matrixS = MAkeTranslateMatrix(S);
	Matrix4x4 matrixR = Multiply(MakeRotateXMatrix(R), Multiply(MakeRotateYMatrix(R), MakeRotateZMatrix(R)));
	Matrix4x4 matrixT = MAkeScaleMatrix(T);
	ans = Multiply(matrixS, Multiply(matrixR, matrixT));
	return ans; }

Vector3 WorldTransform::Transform(Vector3 vector, Matrix4x4 matrix) {
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

Matrix4x4 WorldTransform::MAkeTranslateMatrix(Vector3& vector3) {
	Matrix4x4 ans{0};
	ans.m[0][0] = vector3.x;
	ans.m[1][1] = vector3.y;
	ans.m[2][2] = vector3.z;
	ans.m[3][3] = 1;
	return ans;
}

Matrix4x4 WorldTransform::MAkeScaleMatrix(Vector3& vector3) {
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

Matrix4x4 WorldTransform::MakeRotateXMatrix(Vector3& vector) {
	Matrix4x4 matrix{0};
	matrix.m[1][1] = std::cosf(vector.x);
	matrix.m[1][2] = std::sinf(vector.x);
	matrix.m[2][1] = -std::sinf(vector.x);
	matrix.m[2][2] = std::cosf(vector.x);
	matrix.m[0][0] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

Matrix4x4 WorldTransform::MakeRotateYMatrix(Vector3& vector) {
	Matrix4x4 matrix{0};
	matrix.m[0][0] = std::cosf(vector.y);
	matrix.m[0][2] = -std::sinf(vector.y);
	matrix.m[2][0] = std::sinf(vector.y);
	matrix.m[2][2] = std::cosf(vector.y);
	matrix.m[1][1] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

Matrix4x4 WorldTransform::MakeRotateZMatrix(Vector3& vector) {
	Matrix4x4 matrix{0};
	matrix.m[0][0] = std::cosf(vector.z);
	matrix.m[0][1] = std::sinf(vector.z);
	matrix.m[1][0] = -std::sinf(vector.z);
	matrix.m[1][1] = std::cosf(vector.z);
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}

Matrix4x4 WorldTransform::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
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

