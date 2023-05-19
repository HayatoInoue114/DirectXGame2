#pragma once
#include <Windows.h>
#include <cstdint>
#include "DirectX12.h"
#include "WindowsAPI.h"

class MainRoop
{
public:
	MainRoop(DirectX12* directX12);
	~MainRoop();

	void WindowProcess();

private:
	DirectX12* directX12;

	MSG msg;
	
};



