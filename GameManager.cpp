#include "GameManager.h"

GameManager::GameManager() {
	dataLeft_ = { -0.5f,-0.5f,0.0f,1.0f };
	dataTop_ = { 0.0f,0.5f,0.0f,1.0f };
	dataRight_ = { 0.5f,-0.5f,0.0f,1.0f };
}

GameManager::~GameManager() {

}

void GameManager::Init(DirectX12* directX12, WindowsAPI* windowsAPI) 
{
	
	directX12_ = directX12;
	directX12_->Init(windowsAPI);

	graphicsRenderer_->Dxc();
	graphicsRenderer_->CreateRootSignature(directX12_);
	graphicsRenderer_->InputLayout();
	graphicsRenderer_->BlendState();
	graphicsRenderer_->ResterizerState();
	graphicsRenderer_->BuildShader();
	graphicsRenderer_->CreatePSO(directX12_);

	triangle_->Initialize(directX12_);

	graphicsRenderer_->Viewport();
	graphicsRenderer_->ScissorRect();

}

void GameManager::Update() {
	
	
}

void GameManager::Release() {
	directX12_->Release();
	graphicsRenderer_->Release();

	
}



void GameManager::BeginFrame() {
	directX12_->PreDraw();
	graphicsRenderer_->DrawCall(directX12_);
}

void GameManager::EndFrame() {
	directX12_->PostDraw();
}

void GameManager::Finalize() {
	directX12_->ResourceLeakCheck();
}

void GameManager::Draw() {
	triangle_->Draw(dataLeft_,dataTop_,dataRight_);
}

void GameManager::VariableInit() {
	

	triangle_ = new Triangle;
	triangle_->Initialize(directX12_);
}
