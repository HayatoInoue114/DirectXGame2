#include "TitleScene.h"
#include "../manager/GameManager/GameManager.h"

void TitleScene::Initialize() {
	SetSceneNum(TITLE_SCENE);
	// 入力
	input_ = Input::GetInstance();

	/*Sprite_ = new Sprite;
	Sprite_->Create({ 0,0,0 }, { 1,1 }, { 0,0,0,1 }, MONSTERBALL);*/

	sprite_ = Model::CreateModelFromObjPtr(TITLE);

	transform_.Initialize();
	viewProjection.Initialize();

	transform_.scale_ = { 1.0f,0.5f,0.7f };

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
		transform_.translation_.y += 0.01f;
	}
	else {
		transform_.translation_.y -= 0.01f;
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

void TitleScene::Draw() {
	//sprite_->Draw(transform_);
	sprite_->Draw(transform_, viewProjection, TITLESPRITE);
	//Sprite_->Draw(transform_);
}

void TitleScene::Finalize() {
	//delete sprite_;
}