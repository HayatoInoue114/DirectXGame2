#include "WindowsAPI.h"

void WindowsAPI::Init() {
	WindowClass();
	WindowSize();
	WindowCreate();
}

LRESULT CALLBACK WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WindowsAPI::WindowClass() {
	//ウインドウプロシージャ	
	wc.lpfnWndProc = WindowProc;
	//ウインドウクラス名（なんでもいい）
	wc.lpszClassName = L"メンバ変数";
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウインドウクラスを登録する
	RegisterClass(&wc);
}

void WindowsAPI::WindowSize() {
	wrc = { 0,0,kCliantWidth,kCliantHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
}

void WindowsAPI::WindowCreate() {
	//ウインドウの生成
	hwnd = CreateWindow(
		wc.lpszClassName,		//利用するクラス名
		L"メンバ変数",			//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//よく見るウインドウスタイル
		CW_USEDEFAULT,			//表示X座標（Windowsに任せる）
		CW_USEDEFAULT,			//表示Y座標（Windowsに任せる）
		wrc.right - wrc.left,	//ウインドウ横幅
		wrc.bottom - wrc.top,	//ウインドウ縦幅
		nullptr,				//ウインドウハンドル
		nullptr,				//メニューハンドル
		wc.hInstance,			//インスタンスハンドル
		nullptr
	);

	/*directX12->DebugLayer();*/

	ShowWindow(hwnd, SW_SHOW);
}

