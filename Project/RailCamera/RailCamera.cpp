#include "RailCamera.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotate) {
	worldTransform_.rotation_ = rotate;
	worldTransform_.translation_ = translation;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	Vector3 move = { 0, 0, -0.0f };
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotate);

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	/*ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, 30.0f, 100.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -30, 120);
	ImGui::End();*/
}
