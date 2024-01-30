#include "TitleScene.h"
#include "../manager/GameManager/GameManager.h"

void TitleScene::Initialize() {
	SetSceneNum(TITLE_SCENE);
	// 入力
	input_ = Input::GetInstance();

	sprite_ = Model::CreateModelFromObjPtr(TITLE);

	transform_.Initialize();
	viewProjection.Initialize();

	transform_.scale_ = { 1.0f,0.5f,0.7f };
}

void TitleScene::Update() {
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
}

void TitleScene::Finalize() {
	//delete sprite_;
}