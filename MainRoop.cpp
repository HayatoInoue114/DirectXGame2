#include "MainRoop.h"

MainRoop::MainRoop()
{
	msg = {};
}

MainRoop::~MainRoop()
{
}

void MainRoop::WindowProcess() {
	//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理
		}
}
