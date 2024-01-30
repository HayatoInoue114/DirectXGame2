#pragma once
#include "IScene.h"
#include "../Input/Input.h"
#include "../object/Model/Model.h"

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

	std::unique_ptr<Model> sprite_{};

	WorldTransform transform_{};

	ViewProjection viewProjection;
};