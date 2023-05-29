#include "GameRoop.h"

void GameRoop::Init(DirectX12* directX12, GraphicsRenderer* graphicsRenderer, WindowsAPI* windowsAPI) {
	windowsAPI->WindowClass();
	windowsAPI->WindowSize();
	windowsAPI->WindowCreate();

	directX12->DXGIFactory();
	directX12->Adapter();
	directX12->D3D12Device();

	directX12->Error();
	directX12->Command();
	directX12->SwapChain(windowsAPI);
	directX12->Descriptor();
	


	graphicsRenderer->Dxc();
	graphicsRenderer->CreateRootSignature(directX12);
	graphicsRenderer->InputLayout();
	graphicsRenderer->BlendState();
	graphicsRenderer->ResterizerState();
	graphicsRenderer->BuildShader();
	graphicsRenderer->CreatePSO(directX12);
	graphicsRenderer->CreateVertexResource(directX12);
	graphicsRenderer->CreateVertexBufferView();
	graphicsRenderer->WriteDataToResource();
	graphicsRenderer->Viewport();
	graphicsRenderer->ScissorRect();

	directX12->Fence();
}

void GameRoop::Update(DirectX12* directX12, GraphicsRenderer* graphicsRenderer, WindowsAPI* windowsAPI) {
	directX12->CommandList();
	graphicsRenderer->DrawCall();
	directX12->CommandConfirm();
	directX12->Signal();
	directX12->CommandKick();
	

	directX12->ResourceLeakCheck();
}

void GameRoop::Release(DirectX12* directX12, GraphicsRenderer* graphicsRenderer, WindowsAPI* windowsAPI) {
	directX12->Release();
	graphicsRenderer->Release();
}