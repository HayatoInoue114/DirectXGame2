#pragma once
#include "IScene.h"

class GameManager;
class GameClearScene : public IScene
{
public:
	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	Input* input_{};

	Sprite* sprite_{};

	WorldTransform transform_{};
};