#include "RailCamera.h"

void RailCamera::Initialize(Vector3 translation, Vector3 rotate) {
	worldTransform_.rotate = rotate;
	worldTransform_.translate = translation;
	//viewProjection_.Initialize();
}

void RailCamera::Update() {
	Vector3 move = { 0, 0, -0.0f };
	worldTransform_.translate = Add(worldTransform_.translate, move);

	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	worldTransform_.rotate = Add(worldTransform_.rotate, rotate);

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	//viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	/*ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &viewProjection_.translate.x, 30.0f, 100.0f);
	ImGui::SliderFloat3("rotation", &viewProjection_.rotate.x, -30, 120);
	ImGui::End();*/
}
