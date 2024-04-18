#include "TestScene.h"
#include "../base/GraphicsRenderer/GraphicsRenderer.h"

void TestScene::Initialize() {
	// 入力
	input_ = Input::GetInstance();

	//primitiveManager_.Initialize();
	object_ = std::make_unique<Object3d>();
	object_->Init();
}

void TestScene::Update() {
	//primitiveManager_.Update();

	/*if (!input_->GamePadTrigger(XINPUT_GAMEPAD_B) || input_->PushKeyTrigger(DIK_SPACE)) {
		SetSceneNum(TITLE_SCENE);
	}*/
}

void TestScene::Draw() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	object_->Draw();
	//primitiveManager_.Draw();
}

void TestScene::Finalize() {
	//primitiveManager_.Finalize();
}