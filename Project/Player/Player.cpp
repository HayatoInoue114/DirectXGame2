#include "Player.h"
#include <cassert>
#include <algorithm>

// NULLポインタチェック
Player::Player() {}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition) {
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//reticleTextureHandle_ = Sprite::Create();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	worldSprite2DReticle_.translation_ = { 640, 360, 0 };
	worldSprite2DReticle_.scale_ = { 10,10,0 };
	reticleColor_ = { 1,1,1,1 };

	// スプライト生成
	sprite2DReticle_ = sprite2DReticle_->Create({ 640, 360, 0 }, { 10,10,}, { 1,1,1,1 }, RETICLE);

	worldTransform_.Initialize();
	worldTransform_.translation_ = { playerPosition };
}

void Player::Update(ViewProjection viewProjection) {
	/*worldTransform_.TransferMatrix();*/

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

		spritePosition.x =
			std::clamp(spritePosition.x, 0.0f, (float)kCliantWidth);
		spritePosition.y = std::clamp(
			spritePosition.y, 0.0f, (float)kCliantHeight);
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);

		worldSprite2DReticle_ = sprite2DReticle_->GetWorldTransform();
	}

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 移動限界座標
	const float kMoveLimitX = 17;
	const float kMoveLimitY = 17;

	// 範囲を超えない処理
	worldTransform_.translation_.x =
		std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);

	worldTransform_.translation_.y =
		std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);



	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	float num[3] = {
		worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z };
	ImGui::Begin("Player    DebugCamera : LALT");
	ImGui::SliderFloat3("Player", num, -30, 30);
	ImGui::Text("Rotate : A or D");
	ImGui::Text("Bullet : SPACE");
	ImGui::End();

	worldTransform_.translation_.x = num[0];
	worldTransform_.translation_.y = num[1];
	worldTransform_.translation_.z = num[2];

	Rotate();

	// 自機から3Dレティクルへの距離
	//const float kDistanceplayerTo3DReticle = 50.0f;

	//// 自機のワールド行列の回転を反映
	//offset = Multyply(offset, worldTransform_.matWorld_);
	//// ベクトルの長さを整える
	//offset = Multiply(kDistanceplayerTo3DReticle, Normalize(offset));
	//// 3Dレティクルの座標を設定
	//worldTransform3DReticle_.translation_ = Add(offset, worldTransform_.translation_);

	//Vector3 positionReticle = worldTransform3DReticle_.translation_;
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, kCliantWidth, kCliantHeight, 0, 1);
	/*Matrix4x4 matViewProjectionViewport =
		Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	positionReticle = Transform(positionReticle, matViewProjectionViewport);*/

	// マウス座標（スクリーン座標）を取得する
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	/*HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));*/

	Matrix4x4 matVPV = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector3 posNear = Vector3(sprite2DReticle_->GetPosition().x - 640, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar = Vector3(sprite2DReticle_->GetPosition().x - 640, sprite2DReticle_->GetPosition().y, 1);
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);

	const float kDistanceTestObject = 150.0f;
	worldTransform3DReticle_.translation_.x = posNear.x + mouseDirection.x * kDistanceTestObject;
	worldTransform3DReticle_.translation_.y = posNear.y + mouseDirection.y * kDistanceTestObject;
	worldTransform3DReticle_.translation_.z = posNear.z + mouseDirection.z * kDistanceTestObject;

	worldTransform3DReticle_.translation_.x = std::clamp(
		worldTransform3DReticle_.translation_.x, (float)-kCliantHeight,
		(float)kCliantWidth);

	worldTransform3DReticle_.translation_.y = std::clamp(
		worldTransform3DReticle_.translation_.y, (float)-kCliantHeight,
		(float)kCliantHeight);

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

	/*ImGui::Begin("Player");
	ImGui::Text(
		"2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,+%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text(
		"Far:(%+.2f,+%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();*/
	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw(ViewProjection viewProjection) {
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

// void Player::SetWorldTransform_(WorldTransform worldTransform) {
//	worldTransform_ = {worldTransform};
// }

void Player::Attack() {
	XINPUT_STATE joyState;
	//ゲームパッド未接続状態なら何もせずに抜ける
	if (!Input::GetInstance()->GetJoystickState(joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = kBulletSpeed * Normalize(velocity);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	if (input_->PushKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = kBulletSpeed * Normalize(velocity);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = {
		worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2] };
	return worldPos;
}

Vector3 Player::GetReticleWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = {
		worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		worldTransform3DReticle_.matWorld_.m[3][2] };
	return worldPos;
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawUI() { sprite2DReticle_->Draw(worldSprite2DReticle_); }

Vector2 Player::GetCursorPosition() {
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WindowsAPI::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	return { (float)mousePosition.x, (float)mousePosition.y };
}