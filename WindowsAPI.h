#pragma once
#include <Windows.h>
#include <cstdint>

//クライアント領域のサイズ
const int32_t kCliantWidth = 1280;
const int32_t kCliantHeight = 720;

class WindowsAPI
{
public:
	WindowsAPI();
	~WindowsAPI();

	void Init();

	void WindowProcedure();

	void WindowClass();
	void WindowSize();
	void WindowCreate();

	
public:
	//ウインドウクラスの設定
	WNDCLASS wc;
	
	//ウインドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc;
};



