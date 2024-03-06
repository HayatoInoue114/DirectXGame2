#include "TestScene.h"

void TestScene::Initialize() {
	SetSceneNum(TEST_SCENE);
	// 入力
	input_ = Input::GetInstance();

	primitiveManager_.Initialize();
}

void TestScene::Update() {
	primitiveManager_.Update();
}

void TestScene::Draw() {
	//GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	primitiveManager_.Draw();
}

void TestScene::Finalize() {
}