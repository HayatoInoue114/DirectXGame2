#include "TitleScene.h"
#include "../manager/GameManager/GameManager.h"

void TitleScene::Initialize() {
	SetSceneNum(TITLE_SCENE);
	// 入力
	input_ = Input::GetInstance();

	/*sprite_ = new Sprite;
	sprite_->Create({ 0,0,0 }, { 1,1 }, { 0,0,0,1 }, MONSTERBALL);*/

	sprite_ = std::make_unique<Sprite>();
	sprite_ = Sprite::CreateUniqe({}, {}, { 1,1,1,1 }, TITLESPRITE);
	//sprite_->CreateUniqe({}, {}, {1,1,1,1},TITLESPRITE);

	transform_.Initialize();
	viewProjection.Initialize();

	transform_.scale = { 1.0f,0.5f,0.7f };

	t_ = 0;
	isT_ = 0;
}

void TitleScene::Update() {
	t_++;

	int max = 3;

	if (t_ % 10 == 0) {
		isT_++;
	}

	if (isT_ % 2 == 0) {
		transform_.translate.y += 0.01f;
	}
	else {
		transform_.translate.y -= 0.01f;
	}
	

	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(joyState)) {
		return;
	}
	if (!input_->GamePadTrigger(XINPUT_GAMEPAD_B)) {
		SetSceneNum(GAME_SCENE);
	}
}

//void TitleScene::Draw() {
//	//sprite_->Draw(transform_);
//	
//	//Sprite_->Draw(transform_);
//}

void TitleScene::Draw2D() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	sprite_->Draw();
}

void TitleScene::Draw3D() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	//model_->Draw(1);
}

void TitleScene::Draw() {

	Draw2D();

	Draw3D();

}

void TitleScene::Finalize() {
	//delete sprite_;
}