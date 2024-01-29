#include "GameManager.h"

GameManager::GameManager() {
	// 各シーンの配列
	//sceneArr_[TITLE_SCENE] = new TitleScene();
	//sceneArr_[GAME_SCENE] = new GameScene();
	//sceneArr_[GAMECLEAR_SCENE] = new GameClearScene();
}

GameManager::~GameManager() {
	delete titleScene_;
	delete gameScene_;
	delete gameClearScene_;
}

void GameManager::Initialize() 
{
#pragma region 基盤システムの初期化
	directX12_ = DirectX12::GetInstance();
	directX12_->GetInstance()->Init(WindowsAPI::GetInstance());

	graphicsRenderer_ = GraphicsRenderer::GetInstance();
	graphicsRenderer_->GetInstance()->Initialize(DirectX12::GetInstance());

	light_ = Light::Getinstance();
	light_->Getinstance()->Initialize(DirectX12::GetInstance());
	
	textureManager_ = TextureManager::GetInstance();
	textureManager_->GetInstance()->Initialize();

	modelManager_ = ModelManager::GetInstance();
	modelManager_->GetInstance()->Initialize();

	input_ = Input::GetInstance();
	input_->GetInstance()->Initialize();

#pragma endregion 基盤システムの初期化

	////初期シーンの設定
	//sceneNum_ = TITLE_SCENE;
	//// シーンごとの初期化
	//sceneArr_[sceneNum_]->Initialize();
	
	sceneNum_ = TITLESCENE;

	titleScene_ = new TitleScene();
	titleScene_->Initialize();
	titleSprite_ = Sprite::Create({ 0,0,0 }, { 1,1 }, { 0,0,0,1 }, TITLE);

	gameScene_ = new GameScene();
	gameScene_->Initialize();

	gameClearScene_ = new GameClearScene();
	gameClearScene_->Initialize();

	clearSprite_ = Sprite::Create({ 0,0,0 }, { 1,1 }, { 0,0,0,1 }, CLEAR);

	transform_.Initialize();

	graphicsRenderer_->Viewport();
	graphicsRenderer_->ScissorRect();
}


void GameManager::Update() {
//#pragma region ImGui(もういらないかも)
//	ImGui::ColorEdit3("TriangleColor", colorVolume_);
//	ImGui::SliderFloat3("TriangleColor", colorVolume_, 0.0f, 1.0f);
//	ImGui::SliderFloat3("TriangleScale", scale_, -5, 5);
//	ImGui::SliderFloat3("TriangleRotate", rotate_, -5, 5);
//	ImGui::SliderFloat3("TriangleTranslate", translate_, -1, 1);
//	ImGui::Begin("CommonSettings");
//	if (ImGui::BeginTabBar("CommonTabBar"))
//	{
//		// ライトのImGui
//		if (ImGui::BeginTabItem("Half Lambert")) {
//			light_->AdjustParameter();
//			ImGui::EndTabItem();
//		}
//		ImGui::EndTabBar();
//	}
//	ImGui::End();
//
//	/*color_ = { colorVolume_[0],colorVolume_[1],colorVolume_[2],1.0f };
//	transform_ = { {scale_[0],scale_[1],scale_[2]},{rotate_[0],rotate_[1],rotate_[2]},{translate_[0],translate_[1],translate_[2]} };
//	rotate_[1] += 0.0f;*/
//#pragma endregion ImGui


	input_->GetInstance()->Update();

	// シーンチェック
	//preSceneNum_ = sceneNum_;
	//sceneNum_ = sceneArr_[sceneNum_]->GetSceneNum();

	////シーン変更チェック
	//if (sceneNum_ != preSceneNum_) {
	//	sceneArr_[preSceneNum_]->Finalize();
	//	sceneArr_[sceneNum_]->Initialize();
	//}

#pragma region Update

	switch (sceneNum_)
	{
	case TITLESCENE:
		//titleScene_->Update();
		if (!input_->GamePadTrigger(XINPUT_GAMEPAD_B) || !input_->PushKeyTrigger(DIK_SPACE)) {
			gameScene_->Initialize();
			sceneNum_ = GAMESCENE;
		}
		break;
	case GAMESCENE:
		gameScene_->Update();
		if (gameScene_->GetIsClear()) {
			/*gameScene_->Finalize();*/
			gameClearScene_->Initialize();
			sceneNum_ = GAMECLEARSCENE;
		}

		break;
	case GAMECLEARSCENE:
		//gameClearScene_->Update();
		//clearSprite_->Draw(transform_);
		if (!input_->GamePadTrigger(XINPUT_GAMEPAD_B) || !input_->PushKeyTrigger(DIK_SPACE)) {
			titleScene_->Initialize();
			sceneNum_ = TITLESCENE;
		}
		break;
	default:
		break;
	}

	

	//sceneArr_[sceneNum_]->Update();

#pragma endregion Update

	ImGui::Render();
}


void GameManager::Release() {
	
}



void GameManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	directX12_->PreDraw();
	graphicsRenderer_->DrawCall(directX12_);
}

void GameManager::EndFrame() {
	directX12_->PostDraw();
}

void GameManager::Finalize() {
	directX12_->Release();
	graphicsRenderer_->Release();
	directX12_->ResourceLeakCheck();
}

///////////////////////////////////////////////////////////////Draw//////////////////////////////////
void GameManager::Draw() {
	//sceneArr_[sceneNum_]->Draw();

	switch (sceneNum_)
	{
	case TITLESCENE:
		//titleSprite_->Draw(transform_);
		break;
	case GAMESCENE:
		gameScene_->Draw();
		break;
	case GAMECLEARSCENE:
		//clearSprite_->Draw(transform_);
		break;
	default:
		break;
	}
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


void GameManager::VariableInit() {
}
