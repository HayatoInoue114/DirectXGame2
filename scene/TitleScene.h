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

	void Draw2D();

	void Draw3D();
private:
	Input* input_{};

	std::unique_ptr<Sprite> sprite_{};
	std::unique_ptr<Sprite> black_{};
	//Sprite* sprite_{};

	WorldTransform transform_{};

	ViewProjection viewProjection;

	Vector4 color_;

	float a_{};

	int count_{};

	int t_;
	int isT_;
};