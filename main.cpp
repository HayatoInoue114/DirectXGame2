#include "manager/GameManager/GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<GameManager> gameManager = std::make_unique<GameManager>();
	gameManager->Update();
	return 0;
}