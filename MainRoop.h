#pragma once
#include <Windows.h>
#include <cstdint>


class MainRoop
{
public:
	MainRoop();
	~MainRoop();

	void WindowProcess();

public:
	MSG msg;
};



