#include "PrimitiveManager.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

void PrimitiveManager::Initialize() {

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	player_ = std::make_unique<Player>();
	model_ = Model::CreateModelPtr("cube.obj");
	// 自キャラとレールカメラの親子関係を結ぶ
	model_->SetCamera(railCamera_->GetCamera());
	model_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetCamera(railCamera_->GetCamera());

	Vector3 playerPosition = {0.0f,0.0f,50.0f};

	player_->Initialize(model_.get(), playerPosition);

	domeModel_ = std::make_unique<Model>();
	domeModel_ = Model::CreateModelPtr("skydome.obj");
	domeModel_->SetCamera(railCamera_->GetCamera());

	dome_ = std::make_unique<Skydome>();
	dome_->Initialize(domeModel_.get()/*,railCamera_->GetCamera()*/);



	particle_.Initialize();

	FireAndResetCallback();
}

void PrimitiveManager::Update() {	
	// 敵の発生処理
	UpdateEnemyPopCommands();

	// 敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

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

	railCamera_->Update();

	player_->Update();
	particle_.Update();
	particle_.ImGuiAdjustParameter();
}

void PrimitiveManager::Draw() 
{
	Draw2D();
	Draw3D();
}

void PrimitiveManager::Draw2D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	dome_->Draw();
	player_->Draw();
	//player_->DrawUI();
	
	//player_->DrawUI();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	//// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw();
	}
}

void PrimitiveManager::Draw3D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	//particle_.Draw(railCamera_->GetCamera(), UVCHECKER);
}

void PrimitiveManager::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void PrimitiveManager::EnemySpawn(Vector3 position) {
	Enemy* newEnemy = new Enemy();
	Model* newModel{};
	newModel = Model::CreateModel("enemy.obj");
	newModel->SetColor({ 1,0,0,1 });
	newModel->SetCamera(railCamera_->GetCamera());
	newModel->SetParent(&railCamera_->GetWorldTransform());
	newEnemy->SetPlayer(player_.get());
	newEnemy->Initialize(newModel, position);
	//newEnemy->SetGameScene(this);
	enemies_.push_back(newEnemy);
}

void PrimitiveManager::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./resources./csv./enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void PrimitiveManager::UpdateEnemyPopCommands() {
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

void PrimitiveManager::EnemyFire() {
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
		velocity_ = TransformNormal(velocity_, player_->GetWorldTransform().matWorld_);

		Model* newModel{};
		newModel = Model::CreateModel("cube.obj");
		newModel->SetColor({ 1,1,0,1 });

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(newModel, enemy->GetWorldPosition(), velocity_);
		// 弾を登録する
		AddEnemyBullet(newBullet);
	}
}

void PrimitiveManager::FireAndResetCallback() {
	EnemyFire();

	timedCalls_.push_back(
		new TimedCall(std::bind(&PrimitiveManager::FireAndResetCallback, this), kFireInterval));
 }

void PrimitiveManager::Finalize() {
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
}