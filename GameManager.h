#pragma once
#include "DirectX12.h"
#include "GraphicsRenderer.h"
#include "WindowsAPI.h"
#include "Triangle.h"
#include "ImGuiManager.h"

#define MAXTRIANGLE 1 //三角形の最大数

class GameManager
{
public:
	
	

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
	TriangleData triangleData[MAXTRIANGLE];

	DirectX12* directX12_ = new DirectX12;
	GraphicsRenderer* graphicsRenderer_ = new GraphicsRenderer;
	Triangle** triangle_ = new Triangle* [MAXTRIANGLE];

	float colorVolume_[3] = {};
	Vector4 color_ = {1.0f,0.0f,0.0f,1.0f};

	Transform transform_;
	float scale_[3] = {1.0f,1.0f,0.0f};
	float rotate_[3] = {};
	float translate_[3] = {0.0f,0.0f,0.0f};
};

