#pragma once
#include "DirectX12.h"
#include "GraphicsRenderer.h"
#include "WindowsAPI.h"

class DirectX12;
class GraphicsRenderer;
class WindowsAPI;

class GameRoop
{
public:
	void Init(DirectX12* directX12, GraphicsRenderer* graphicsRenderer, WindowsAPI* windowsAPI);
	void Update(DirectX12* directX12, GraphicsRenderer* graphicsRenderer, WindowsAPI* windowsAPI);
	void Release(DirectX12* directX12, GraphicsRenderer* graphicsRenderer, WindowsAPI* windowsAPI);

private:
	DirectX12* directX12;
	GraphicsRenderer* graphicsRenderer;
	WindowsAPI* windowsAPI;
};

