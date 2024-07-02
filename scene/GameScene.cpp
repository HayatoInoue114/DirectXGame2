#include "GameScene.h"
//#include "AxisIndicator.h"
#include "../manager/TextureManager/TextureManager.h"
#include <cassert>
#include <fstream>
#include "../manager/ModelManager/ModelManager.h"
#include "../base/GraphicsRenderer/GraphicsRenderer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	//delete modelSkydome_;
	//delete railCamera_;
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
	// ビュー(カメラ)の解放
	//viewProjection.constBuff_.ReleaseAndGetAddressOf();

	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}

void GameScene::Initialize() {
	input_ = Input::GetInstance();

	model = Model:: CreateModelFromObj(CUBE);

	worldTransform_.Initialize();
	worldTransform_.scale = { 10,10,1 };

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateModelFromObjPtr("skydome");

	//// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	// 自キャラの生成
	player_ = std::make_unique<Player>();

	//textureHandle = PLAYER;
	// 自キャラの初期化
	playerModel_ = Model::CreateModelFromObjPtr("cube");

	

	LoadEnemyPopData();


	// 天球の生成
	//skydome_ = new Skydome();
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_.get(),railCamera_->GetCamera());

	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	Vector3 playerPosition(0, 0, 50.0f);

	player_->Initialize(playerModel_.get(), playerPosition);

	dedCount_ = 0;

	flameCount_ = 0;

	isClear_ = false;

	//blackSprite_ = Sprite::CreateUnique({ 0,0,0 }, { 1280,720 }, { 1,1,1,1 }, BLACK);

	color_ = { 1,1,1,1 };

	FireAndResetCallback();

	
}

void GameScene::Update() {
	flameCount_++;
	if (dedCount_ >= 5 || flameCount_ >= 50000) {
		enemyPopCommands = {};
		isClear_ = true;
	}

	// レールカメラの更新
	railCamera_->Update();


	
	// 敵の発生処理
	UpdateEnemyPopCommands();

	// 敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// 自キャラの更新
	player_->Update();

	// 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return true;
		}
		return false;
	});

	// 範囲for文でリストの全要素について回す
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->SetPlayer(player_.get());

		bullet->Update();
	}

	// 当たり判定
	CheckAllCollisions();

	// 天球の更新
	skydome_->Update();

	color_.w -= 0.1f;

}

void GameScene::Draw2D() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	player_->DrawUI();

	skydome_->Draw();

	player_->Draw();


	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw();
	}

}

void GameScene::Draw3D() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
}


void GameScene::Draw() {

	Draw2D();
	Draw3D();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	//Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	/// 
	
	

	// スプライト描画後処理
	//Sprite::PostDraw();
	// 深度バッファクリア
	//dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	//Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//skydome_->Draw(viewProjection);
	

	// 3Dオブジェクト描画後処理
	//Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	//Sprite::PreDraw(commandList);

	
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	//Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;
	// 敵リストの取得
	//const std::list<Enemy*>& enemy = enemies_;

#pragma region 自キャラと敵弾の当たり判定
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		Vector3 distance = Subtract(posA, posB);
		if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <= 3.0f * 3.0f) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemies_) {
		posA = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetWorldPosition();

			Vector3 distance = Subtract(posA, posB);
			if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <=
				3.0f * 3.0f) {
				enemy->OnCollision();
				bullet->OnCollision();
				dedCount_++;
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* eBullet : enemyBullets) {
			posA = playerBullet->GetWorldPosition();
			posB = eBullet->GetWorldPosition();

			Vector3 distance = Subtract(posA, posB);
			if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <=
				3.0f * 3.0f) {
				playerBullet->OnCollision();
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemhyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemhyBullet);
}

void GameScene::EnemySpawn(Vector3 position) {
	Enemy* newEnemy = new Enemy();
	Model* newModel{};
	newModel = Model::CreateModelFromObj(ENEMY);
	newModel->SetColor({ 1,0,0,1 });
	newEnemy->SetPlayer(player_.get());
	newEnemy->Initialize(newModel, position);
	newEnemy->SetGameScene(this);
	enemies_.push_back(newEnemy);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./resources./csv./enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWaiting_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			isWaiting_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数名
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の戦闘文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemySpawn(Vector3(x, y, z));

		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaiting_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemyFire() {
	// 差分ベクトルを求める
	for (Enemy* enemy : enemies_) {
		assert(player_);

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity_ = { 1, 1, kBulletSpeed };

		velocity_ = Subtract(player_->GetWorldPosition(), enemy->GetWorldPosition());
		// ベクトルの正規化
		velocity_ = Normalize(velocity_);

		// ベクトルの長さを速さに合わせる
		//velocity_ = kBulletSpeed * velocity_;

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);

		Model* newModel{};
		newModel = Model::CreateModelFromObj(CUBE);
		newModel->SetColor({ 1,1,0,1 });

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(newModel, enemy->GetWorldPosition(), velocity_);
		// 弾を登録する
		AddEnemyBullet(newBullet);
	}
}

void GameScene::FireAndResetCallback() {
	EnemyFire();

	timedCalls_.push_back(
		new TimedCall(std::bind(&GameScene::FireAndResetCallback, this), kFireInterval));
}

void GameScene::Finalize() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
	// ビュー(カメラ)の解放
	//viewProjection.constBuff_.ReleaseAndGetAddressOf();

	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
}