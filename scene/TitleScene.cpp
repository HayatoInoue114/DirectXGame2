#include "TitleScene.h"
#include "../manager/GameManager/GameManager.h"

void TitleScene::Initialize() {
	SetSceneNum(TITLE_SCENE);
	// 入力
	input_ = Input::GetInstance();

	sprite_ = Sprite::Create({0,0,0},{1,1},{0,0,0,1},TITLE);

	transform_.Initialize();
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
}

void TitleScene::Finalize() {
	//delete sprite_;
}