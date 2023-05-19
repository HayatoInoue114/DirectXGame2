#pragma once
#include <Windows.h>
#include <cstdint>
#include "DirectX12.h"



class WindowsAPI
{
public:

	//クライアント領域のサイズ
	static const int32_t kCliantWidth = 1280;
	static const int32_t kCliantHeight = 720;


	WindowsAPI();
	~WindowsAPI();

	void Init();

	

	void WindowClass();
	void WindowSize();
	void WindowCreate();

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	HWND GetHwnd() const { return hwnd; }

public:
	//ウインドウクラスの設定
	WNDCLASS wc;
	
	//ウインドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc;

	HWND hwnd;

	DirectX12* directX12;
};



