#include "GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	CoInitializeEx(0, COINIT_MULTITHREADED);
	
	WindowsAPI* windowsAPI = new WindowsAPI();
	DirectX12* directX12 = new DirectX12;
	GameManager* gameManager = new GameManager;

	gameManager->VariableInit();

	gameManager->Init(directX12,windowsAPI);

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