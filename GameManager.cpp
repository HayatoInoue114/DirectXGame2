#include "GameManager.h"

void GameManager::Init(DirectX12* directX12, WindowsAPI* windowsAPI) 
{
	
	directX12_ = directX12;
	directX12_->Init(windowsAPI);

	graphicsRenderer_->Initialize(directX12_);

	for (int i = 0; i < 3; i++) {
		colorVolume_[i] = 1.0f;
		scale_[i] = 1.0f;
		rotate_[i] = 0.0f;
		translate_[i] = 0.0f;
	}

	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i] = new Triangle;
		triangle_[i]->Initialize(directX12_, triangleData[i]);
	}

	for (int i = 0; i < MAXSPRITE; i++) {
		sprite_[i] = new Sprite;
		sprite_[i]->Initialize(directX12_);
	}

	graphicsRenderer_->Viewport();
	graphicsRenderer_->ScissorRect();
}

void GameManager::Update() {
	ImGui::ColorEdit3("TriangleColor", colorVolume_);
	ImGui::SliderFloat3("TriangleColor", colorVolume_, 0.0f, 1.0f);
	ImGui::SliderFloat3("TriangleScale", scale_, -5, 5);
	ImGui::SliderFloat3("TriangleRotate", rotate_, -5, 5);
	ImGui::SliderFloat3("TriangleTranslate", translate_, -1, 1);
	
	color_ = { colorVolume_[0],colorVolume_[1],colorVolume_[2],1.0f };
	transform_ = { {scale_[0],scale_[1],scale_[2]},{rotate_[0],rotate_[1],rotate_[2]},{translate_[0],translate_[1],translate_[2]} };
	rotate_[1] += 0.02f;

	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i]->Update(transform_,color_);
	}

	for (int i = 0; i < MAXSPRITE; i++) {
		sprite_[i]->Update(transform_, color_);
	}
	

	ImGui::Render();
}

void GameManager::Release() {
	directX12_->Release();
	graphicsRenderer_->Release();
	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i]->Release();
	}
	//for (int i = 0; i < MAXSPRITE; i++) {
	//	sprite_[i]->Release();
	//}
}



void GameManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
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
	for (int i = 0; i < MAXSPRITE; i++) {
		sprite_[i]->Draw();
	}
	
}

void GameManager::VariableInit() {
	
	for (int i = 0; i < MAXTRIANGLE; i++) {
		triangleData[i].Left.position = { -0.5f,-0.5f,0.0f,1.0f };
		triangleData[i].Left.texcoord = { 0.0f,1.0f };

		triangleData[i].Top.position = { 0.0f,0.5f,0.0f,1.0f };
		triangleData[i].Top.texcoord = { 0.5f,0.0f };

		triangleData[i].Right.position = { 0.5f,-0.5f,0.0f,1.0f };
		triangleData[i].Right.texcoord = { 1.0f,1.0f };
	}

	/*for (int i = 0; i < MAXTRIANGLE; i++) {
		triangle_[i] = new Triangle;
		triangle_[i]->Initialize(directX12_,triangleData[i]);
	}*/
}
