#pragma once
#include <Windows.h>
#include <cstdint>



class WindowsAPI
{
public:
	WindowsAPI();
	~WindowsAPI();

	void Init();

	

	void WindowClass();
	void WindowSize();
	void WindowCreate();

	void WindowProc();

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	//ウインドウクラスの設定
	WNDCLASS wc;
	
	//ウインドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc;

	//クライアント領域のサイズ
	const int32_t kCliantWidth = 1280;
	const int32_t kCliantHeight = 720;
};



