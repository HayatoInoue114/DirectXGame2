#include "PrimitiveManager.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

void PrimitiveManager::Initialize() {

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	player_ = std::make_unique<Player>();
	model_ = Model::CreateModelFromObjPtr(PLAYER);
	// 自キャラとレールカメラの親子関係を結ぶ
	model_->SetCamera(railCamera_->GetCamera());
	model_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetCamera(railCamera_->GetCamera());

	Vector3 playerPosition = {0.0f,0.0f,50.0f};

	player_->Initialize(model_.get(), playerPosition);

	domeModel_ = std::make_unique<Model>();
	domeModel_ = Model::CreateModelFromObjPtr(SKYDOME);
	domeModel_->SetCamera(railCamera_->GetCamera());

	dome_ = std::make_unique<Skydome>();
	dome_->Initialize(domeModel_.get()/*,railCamera_->GetCamera()*/);



	particle_.Initialize();
}

void PrimitiveManager::Update() {	


	railCamera_->Update();

	player_->Update();
	particle_.Update();
	particle_.ImGuiAdjustParameter();
}

void PrimitiveManager::Draw() 
{
	Draw2D();
	Draw3D();
}

void PrimitiveManager::Draw2D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	dome_->Draw();
	player_->Draw();

}

void PrimitiveManager::Draw3D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	particle_.Draw(railCamera_->GetCamera(), UVCHECKER);
}
