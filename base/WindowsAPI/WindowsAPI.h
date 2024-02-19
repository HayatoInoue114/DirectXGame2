#pragma once
#include <Windows.h>
#include <cstdint>
#include "../../manager/ImGuiManager/ImGuiManager.h"
//#include "DirectX12.h"

//クライアント領域のサイズ
static const int32_t kCliantWidth = 1280;
static const int32_t kCliantHeight = 720;

//class DirectX12;

class WindowsAPI
{
public:
	static WindowsAPI* GetInstance();

	void Init();

	HWND GetHwnd() const { return hwnd; }
	
private:
	void WindowClass();
	void WindowSize();
	void WindowCreate();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	//ウインドウクラスの設定
	WNDCLASS wc{};
	
	//ウインドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc{};

	HWND hwnd = {};
};



