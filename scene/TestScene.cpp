#include "TestScene.h"
#include "../base/GraphicsRenderer/GraphicsRenderer.h"

void TestScene::Initialize() {
	// 入力
	input_ = Input::GetInstance();

	primitiveManager_.Initialize();

	/*particle_ = std::make_unique<Particle>();
	particle_->Initialize();*/
}

void TestScene::Update() {
	primitiveManager_.Update();
}

void TestScene::Draw() {
	//GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	//particle_->Draw(CIRCLE);
	primitiveManager_.Draw();
}

void TestScene::Finalize() {
}