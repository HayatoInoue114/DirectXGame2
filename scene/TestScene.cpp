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

	if (!input_->GamePadTrigger(XINPUT_GAMEPAD_B) || input_->PushKeyTrigger(DIK_SPACE)) {
		SetSceneNum(GAMECLEAR_SCENE);
	}
}

void TestScene::Draw() {
	
	//particle_->Draw(CIRCLE);
	primitiveManager_.Draw();
}

void TestScene::Finalize() {
}