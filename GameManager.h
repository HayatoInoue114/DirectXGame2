#pragma once
#include "DirectX12.h"
#include "GraphicsRenderer.h"
#include "WindowsAPI.h"
#include "Triangle.h"

class DirectX12;
class GraphicsRenderer;
class WindowsAPI;

class GameManager
{
public:
	
	DirectX12* directX12_ = new DirectX12;
	GraphicsRenderer* graphicsRenderer_ = new GraphicsRenderer;
	Triangle* triangle_ = new Triangle;

	/*void PreInit(GraphicsRenderer* graphicsRenderer);*/
	void Init(DirectX12* directX12, WindowsAPI* windowsAPI);
	void Update();
	void Release();

	

	void BeginFrame();

	void EndFrame();

	void Finalize();

	void Draw();

	void VariableInit();
private:
	Vector4 dataLeft_;
	Vector4 dataTop_;
	Vector4 dataRight_;

	/*DirectX12* directX12;
	GraphicsRenderer* graphicsRenderer;
	WindowsAPI* windowsAPI;*/
};

