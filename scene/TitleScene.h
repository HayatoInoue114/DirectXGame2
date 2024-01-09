#pragma once
#include "IScene.h"
#include "../Input/Input.h"
#include "../object/Sprite/Sprite.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	Input* input_{};

	Sprite* sprite_{};

	WorldTransform transform_{};
};