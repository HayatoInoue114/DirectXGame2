#include "TestScene.h"
#include "../base/GraphicsRenderer/GraphicsRenderer.h"

void TestScene::Initialize() {
	SetSceneNum(TEST_SCENE);
	// 入力
	input_ = Input::GetInstance();

	//primitiveManager_.Initialize();

	particle_ = std::make_unique<Particle>();
	particle_->Initialize();
}

void TestScene::Update() {
	//primitiveManager_.Update();
}

void TestScene::Draw() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	particle_->Draw(0);
	//primitiveManager_.Draw();
}

void TestScene::Finalize() {
}