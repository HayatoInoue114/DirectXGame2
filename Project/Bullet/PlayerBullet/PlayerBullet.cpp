#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() { velocity_ = {}; }

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	/*textureHandle_ = TextureManager::GetInstance()->LoadTexture("godest.png");*/

	//textureHandle_ = PLAYERBULLET;

	worldTransform_.Initialize();

	/*model_->SetCamera(camera_);
	model_->SetParent(&camera_->GetWorldTransform());*/

	// 引数で受け取った初期座標をセット
	worldTransform_.translate = { position };
	model_->SetWorldTransform(worldTransform_);

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	// 座標を移動させる（1フレーム分の移動量を足しこむ)
	worldTransform_.translate = Add(worldTransform_.translate, velocity_);
	model_->SetWorldTransform(worldTransform_);

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Bullet");
	ImGui::DragFloat3("translate", &worldTransform_.translate.x);
	ImGui::End();
}

void PlayerBullet::Draw() {
	model_->Draw();
}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = {
		worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2] };
	return worldPos;
}

void PlayerBullet::OnCollision() { isDead_ = true; }