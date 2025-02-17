#include "Player.h"
#include <cassert>
#include <algorithm>
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

// NULLポインタチェック
Player::Player() { camera_ = {}; }

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, Vector3 playerPosition) {
	//assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	object_ = std::make_unique<Object3d>();
	object_->Init(model);

	//reticleTextureHandle_ = Sprite::Create();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//worldSprite2DReticle_.parent_ = &camera_->GetWorldTransform();
	//worldSprite2DReticle_ = camera_->GetWorldTransform();
	worldSprite2DReticle_.scale = { 10,10,0 };
	reticleColor_ = { 1,1,1,1 };

	// スプライト生成
	/*sprite2DReticle_ = new Sprite;
	sprite2DReticle_ = sprite2DReticle_->Create({ 640, 360, 0 }, { 10,10 }, { 0,0,0,1 }, RETICLE);*/
	//sprite2DReticle_ = Sprite::CreateUnique({ 640, 360, 50 }, { 100,100 }, { 1,1,1,1 }, RETICLE);

	worldTransform_.Initialize();
	object_->SetTranslate(playerPosition);
	worldTransform_.translate = playerPosition;

	reticleModel_ = Model::CreateModelPtr("cube.obj");
	reticleModel_->SetCamera(camera_);
	reticleModel_->SetParent(&camera_->GetWorldTransform());
	Vector3 pos = worldTransform3DReticle_.translate;
	pos.z = 100.0f;
	reticleModel_->SetWorldTransform(worldTransform3DReticle_);
	reticleModel_->SetTranslate(pos);
}

void Player::Update() {
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

	Vector3 rotate = {};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;

		rotate.z -= (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
	}

	// スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	//Vector2 spritePosition = { worldSprite2DReticle_.translate.x,worldSprite2DReticle_.translate.y };


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
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 座標移動（ベクトルの加算）
	worldTransform_.translate = Add(worldTransform_.translate, move);

	worldTransform_.rotate = Add(worldTransform_.rotate, rotate);

	worldTransform_.rotate.z = std::clamp(worldTransform_.rotate.z, -1.0f, 1.0f);

	worldTransform_.matWorld_ = MakeAffineMatrix(
		worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);

	// 移動限界座標
	const float kMoveLimitX = 17;
	const float kMoveLimitY = 10;

	// 範囲を超えない処理
	worldTransform_.translate.x =
		std::clamp(worldTransform_.translate.x, -kMoveLimitX, kMoveLimitX);

	worldTransform_.translate.y =
		std::clamp(worldTransform_.translate.y, -kMoveLimitY, kMoveLimitY);

	object_->SetWorldTransform(worldTransform_);

	if (input_->PushKeyTrigger(DIK_SPACE)) {
		static int a = 0;
		a++;
	}

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	//Rotate();

	//// 自機から3Dレティクルへの距離
	//const float kDistanceplayerTo3DReticle = 50.0f;

	//// 自機のワールド行列の回転を反映
	//offset = Multiply(offset, worldTransform_.matWorld_);
	//// ベクトルの長さを整える
	//offset = Multiply(Normalize(offset), kDistanceplayerTo3DReticle);
	//// 3Dレティクルの座標を設定
	//worldTransform3DReticle_.translate = Add(offset, worldTransform_.translate);

	//Vector3 positionReticle = worldTransform3DReticle_.translate;
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, kCliantWidth, kCliantHeight, 0, 1);
	/*Matrix4x4 matViewProjectionViewport =
		Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	positionReticle = Transform(positionReticle, matViewProjectionViewport);*/

	// マウス座標（スクリーン座標）を取得する
	/*POINT mousePosition;
	GetCursorPos(&mousePosition);*/

	//クライアントエリア座標に変換する
	/*HWND hwnd = WindowsAPI::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));*/

	/*Matrix4x4 matVPV = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector3 posNear = Vector3(sprite2DReticle_->GetPosition().x - 640, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar = Vector3(sprite2DReticle_->GetPosition().x - 640, sprite2DReticle_->GetPosition().y, 1);
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);

	const float kDistanceTestObject = 150.0f;
	worldTransform3DReticle_.translate.x = posNear.x + mouseDirection.x * kDistanceTestObject;
	worldTransform3DReticle_.translate.y = posNear.y + mouseDirection.y * kDistanceTestObject;
	worldTransform3DReticle_.translate.z = posNear.z + mouseDirection.z * kDistanceTestObject;

	worldTransform3DReticle_.translate.x = std::clamp(
		worldTransform3DReticle_.translate.x, (float)-kCliantHeight,
		(float)kCliantWidth);

	worldTransform3DReticle_.translate.y = std::clamp(
		worldTransform3DReticle_.translate.y, (float)-kCliantHeight,
		(float)kCliantHeight);

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();*/

	// キャラクター攻撃処理
	Attack();

	sprite2DReticle_->Update();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	ImGui::Begin("Player");
	ImGui::DragFloat3("translate", &worldTransform_.translate.x);
	ImGui::DragFloat3("reticle", &worldTransform3DReticle_.translate.x);
	ImGui::End();
}

void Player::Draw() {
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		//bullet->SetCamera(camera_);
		bullet->Draw();
	}

	reticleModel_->Draw();

	object_->Draw();
}

void Player::Attack() {
	XINPUT_STATE joyState{};
	//ゲームパッド未接続状態なら何もせずに抜ける
	//if (!Input::GetInstance()->GetJoystickState(joyState)) {
	//	return;
	//}

	if (!Input::GetInstance()->GamePadTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translate, GetWorldPosition());
		//velocity = Normalize(velocity) * kBulletSpeed;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		Model* model{};
		model = Model::CreateModel("cube.obj");
		//Object3d* object{};
		

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	if (input_->PushKeyTrigger(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		//velocity = Subtract(worldTransform3DReticle_.translate, GetWorldPosition());
		velocity = kBulletSpeed * Normalize(velocity);


		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		Model* model{};
		model = Model::CreateModel("cube.obj");
		

		Vector3 newPos = GetWorldPosition();

		model->SetCamera(camera_);
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model, newPos, velocity);
		
		//model->SetParent(&camera_->GetWorldTransform());
		newBullet->SetCamera(camera_);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotate.y += kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotate.y -= kRotSpeed;
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

void Player::DrawUI() { 
	sprite2DReticle_->Draw(); 
}

Vector2 Player::GetCursorPosition() {
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WindowsAPI::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	return { (float)mousePosition.x, (float)mousePosition.y };
}