#pragma once
#include <Windows.h>
#include <cstdint>
#include "ImGuiManager.h"
//#include "DirectX12.h"

//クライアント領域のサイズ
static const int32_t kCliantWidth = 1280;
static const int32_t kCliantHeight = 720;

//class DirectX12;

class WindowsAPI
{
public:

	void Init();

	void WindowClass();
	void WindowSize();
	void WindowCreate();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	HWND GetHwnd() const { return hwnd; }

private:
	//ウインドウクラスの設定
	WNDCLASS wc{};
	
	//ウインドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc;

	HWND hwnd = {};
};



