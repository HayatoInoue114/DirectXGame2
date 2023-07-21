#include "GameManager.h"



void GameManager::Init(DirectX12* directX12, WindowsAPI* windowsAPI) 
{
	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i] = new Triangle;
	}
	

	directX12_ = directX12;
	directX12_->Init(windowsAPI);

	graphicsRenderer_->Dxc();
	graphicsRenderer_->CreateRootSignature(directX12_);
	graphicsRenderer_->InputLayout();
	graphicsRenderer_->BlendState();
	graphicsRenderer_->ResterizerState();
	graphicsRenderer_->BuildShader();
	graphicsRenderer_->CreatePSO(directX12_);

	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i]->Initialize(directX12_, triangleData[i].Left_, triangleData[i].Top_, triangleData[i].Right_);
	}

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
	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i]->Draw();
	}
	
}

void GameManager::VariableInit() {
	
	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangleData[i].Left_ = { -0.5f,-0.5f + i * 0.1f,0.0f,1.0f };
		triangleData[i].Top_ = { 0.0f,0.5f + i * 0.1f,0.0f,1.0f };
		triangleData[i].Right_ = { 0.5f,-0.5f + i * 0.1f,0.0f,1.0f };
	}

	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i] = new Triangle;
		triangle_[i]->Initialize(directX12_,triangleData[i].Left_, triangleData[i].Top_, triangleData[i].Right_);
	}
}
