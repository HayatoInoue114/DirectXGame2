#include "TitleScene.h"
#include "../manager/GameManager/GameManager.h"

void TitleScene::Initialize() {
	//SetSceneNum(TITLE_SCENE);
	// 入力
	input_ = Input::GetInstance();

	/*sprite_ = new Sprite;
	sprite_->Create({ 0,0,0 }, { 1,1 }, { 0,0,0,1 }, MONSTERBALL);*/

	sprite_ = Sprite::CreateUnique({0,0,0}, {1280,720}, {1,1,1,1},TITLESPRITE);
	black_ = Sprite::CreateUnique({ 0,0,0 }, { 1280,720 }, { 1,1,1,1 }, BLACK);


	transform_.Initialize();
	//viewProjection.Initialize();

	transform_.scale = { 1.0f,0.5f,0.7f };

	t_ = 0;
	isT_ = 0;

	color_ = { 1,1,1,0 };
	a_ = 0;
	count_ = 0;
}

void TitleScene::Update() {
	t_++;

	if (t_ % 10 == 0) {
		isT_++;
	}

	if (isT_ % 2 == 0) {
		transform_.translate.y += 1;
	}
	else {
		transform_.translate.y -= 1;
	}


	
	sprite_->SetPosition({ transform_.translate.x,transform_.translate.y });

	color_.w = a_;
	black_->SetColor(color_);

	black_->Update();
	sprite_->Update();

	// ゲームパッド未接続なら何もせず抜ける
	/*if (!Input::GetInstance()->GetJoystickState(joyState)) {
		return;
	}*/
	if (!input_->GamePadTrigger(XINPUT_GAMEPAD_B) || input_->PushKeyTrigger(DIK_SPACE)) {
		count_ = 1;
	}

	if (count_ == 1) {
		if (t_ % 3 == 0) {
			a_ += 0.1f;
		}
	}

	if (a_ >= 1) {
		SetSceneNum(TEST_SCENE);
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
	//black_->Draw();
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