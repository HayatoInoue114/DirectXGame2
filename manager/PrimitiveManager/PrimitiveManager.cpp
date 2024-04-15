#include "PrimitiveManager.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

void PrimitiveManager::Initialize() {
	/*camera_ = std::make_unique<Camera>();
	camera_->Initialize();*/

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	/*primitive_ = std::make_unique<Primitive>();
	model_ = Model::CreateModelFromObjPtr(PLAYER);
	primitive_->SetModel(model_.get());
	primitive_->SetCamera(railCamera_->GetCamera());
	primitive_->SetParent(&railCamera_->GetWorldTransform());
	Vector3 pos = { 0,0,30 };
	primitive_->Initialize(pos);*/

	player_ = std::make_unique<Player>();
	model_ = Model::CreateModelFromObjPtr(PLAYER);
	// 自キャラとレールカメラの親子関係を結ぶ
	model_->SetCamera(railCamera_->GetCamera());
	model_->SetParent(&railCamera_->GetWorldTransform());

	Vector3 playerPosition = {0.0f,0.0f,50.0f};

	player_->Initialize(model_.get(), playerPosition);

	//primitive2_ = std::make_unique<Primitive>();
	//primitive2_->SetCamera(camera_.get());
	//primitive2_->Initialize();
	//model2_ = model2_->CreateModelFromObjPtr(ENEMY);
	//primitive2_->SetModel(model2_.get());

	domeModel_ = std::make_unique<Model>();
	domeModel_ = Model::CreateModelFromObjPtr(SKYDOME);

	dome_ = std::make_unique<Skydome>();
	dome_->Initialize(domeModel_.get(),railCamera_->GetCamera());



	particle_.Initialize();
}

void PrimitiveManager::Update() {
	//camera_->Update();
	//railCamera_->SetCamera(camera_.get());
	

	
	railCamera_->Update();

	player_->Update();
	//primitive_->Update();
	/*primitive2_->SetCamera(camera_.get());
	primitive2_->Update();*/
	particle_.Update();
	particle_.ImGuiAdjustParameter();
}

void PrimitiveManager::Draw() {
	Draw2D();
	Draw3D();
}

void PrimitiveManager::Draw2D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	dome_->Draw();
	player_->Draw();
	//primitive_->Draw();
	//primitive2_->Draw();
	
}

void PrimitiveManager::Draw3D()
{
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(1);
	//particle_.Draw(camera_.get(), UVCHECKER);
}
