#include "GameManager.h"

GameManager::GameManager() {
	// 各シーンの配列
	sceneArr_[TITLE_SCENE] = new TitleScene();
	sceneArr_[GAME_SCENE] = new GameScene();
	sceneArr_[GAMECLEAR_SCENE] = new GameClearScene();
	sceneArr_[TEST_SCENE] = new TestScene();
}

GameManager::~GameManager() {
	delete titleScene_;
	delete gameScene_;
	delete gameClearScene_;
	delete testScene_;
}

void GameManager::Initialize()
{
#pragma region 基盤システムの初期化
	directX12_ = DirectX12::GetInstance();
	directX12_->GetInstance()->Init();

	graphicsRenderer_ = GraphicsRenderer::GetInstance();
	graphicsRenderer_->GetInstance()->Initialize();

	light_ = Light::Getinstance();
	light_->Getinstance()->Initialize();

	textureManager_ = TextureManager::GetInstance();
	textureManager_->GetInstance()->Initialize();

	input_ = Input::GetInstance();
	input_->GetInstance()->Initialize();

#pragma endregion 基盤システムの初期化

	////////////   *初期シーンの設定*   ////////////
	sceneNum_ = TEST_SCENE;
	// シーンごとの初期化
	sceneArr_[sceneNum_]->Initialize();

	graphicsRenderer_->Viewport();
	graphicsRenderer_->ScissorRect();
}


void GameManager::Update() {
	// 初期化
	Initialize();

	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// 描画前の処理
			BeginFrame();

			// シーンチェック
			preSceneNum_ = sceneNum_;
			//sceneNum_ = sceneArr_[sceneNum_]->GetSceneNum();

			input_->Update();

			//シーン変更チェック
			if (sceneNum_ != preSceneNum_) {
				sceneArr_[preSceneNum_]->Finalize();
				sceneArr_[sceneNum_]->Initialize();
			}

			///
			/// 更新処理
			/// 	
			sceneArr_[sceneNum_]->Update();

			// ImGuiのパラメータを入れている

			///
			/// 描画処理
			/// 
			sceneArr_[sceneNum_]->Draw();
			ImGui::Render();
			// 描画後の処理
			EndFrame();

		}
	}
	// 解放処理
	Finalize();

}


void GameManager::Release() {

}

void GameManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	directX12_->PreDraw();
	graphicsRenderer_->DrawCall();
}

void GameManager::EndFrame() {
	directX12_->PostDraw();
}

void GameManager::Finalize() {
}
