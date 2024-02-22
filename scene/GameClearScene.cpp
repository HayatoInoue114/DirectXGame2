#include "GameClearScene.h"
#include "../manager/GameManager/GameManager.h"
#include "../base/WindowsAPI/WindowsAPI.h"

void GameClearScene::Initialize() {
	input_ = Input::GetInstance();

	sprite_ = sprite_->Create({ 0,0,0 }, { 1,1 }, { 0,0,0,1 }, CLEAR);

	transform_.Initialize();
}

void GameClearScene::Update() {
	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(joyState)) {
		return;
	}
	if (!input_->GamePadTrigger(XINPUT_GAMEPAD_B)) {
		SetSceneNum(TITLE_SCENE);
	}
}

void GameClearScene::Draw() {
	//sprite_->Draw(transform_);
}

void GameClearScene::Finalize() {
	delete sprite_;
}