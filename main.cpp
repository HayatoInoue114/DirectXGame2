#include "GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	CoInitializeEx(0, COINIT_MULTITHREADED);

	GameManager* gameManager = new GameManager;

	gameManager->VariableInit();

	gameManager->Initialize();

	MSG msg{};
	//Windowにメッセージが来てたら最優先で処理させる
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理
			gameManager->BeginFrame();

			gameManager->Update();

			gameManager->Draw();

			gameManager->EndFrame();
			
		}
	}

	gameManager->Finalize();
	CoUninitialize();
	return 0;
}